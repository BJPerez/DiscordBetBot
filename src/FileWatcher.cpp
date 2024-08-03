#include "FileWatcher.h"

#include <algorithm>

#include <filesystem>
#include <thread>

FileWatcher::FileWatcher(std::string pathToFolder, const std::chrono::minutes delay, Callback onFileCreatedCallback) noexcept:
	m_FolderToWatch(std::move(pathToFolder)), m_DelayBetweenChecks(delay), m_OnFileCreatedCallback(std::move(onFileCreatedCallback))
{
}

FileWatcher::FileWatcher(FileWatcher&& other) noexcept: m_FolderToWatch(std::move(other.m_FolderToWatch)), m_DelayBetweenChecks(other.m_DelayBetweenChecks), m_Files(std::move(other.m_Files)),
	m_IsRunning(other.m_IsRunning.load()), m_OnFileCreatedCallback(std::move(other.m_OnFileCreatedCallback)), m_WorkingThread(std::move(other.m_WorkingThread))
{
	
}

FileWatcher::~FileWatcher()
{
	if (m_IsRunning.load())
	{
		Stop();
		m_WorkingThread.join();
	}
}

FileWatcher& FileWatcher::operator=(FileWatcher&& other) noexcept
{
	m_FolderToWatch = std::move(other.m_FolderToWatch);
	m_DelayBetweenChecks = other.m_DelayBetweenChecks;
	m_Files = std::move(other.m_Files);
	m_IsRunning.store(other.m_IsRunning.load());
	m_OnFileCreatedCallback = std::move(other.m_OnFileCreatedCallback);
	m_WorkingThread = std::move(other.m_WorkingThread);

	return *this;
}

void FileWatcher::Run(const bool returnAfter)
{
	if (bool expected = false; 
		m_IsRunning.compare_exchange_strong(expected, true))
	{
		m_WorkingThread = std::thread(&FileWatcher::RunInternal, this);
		if (!returnAfter)
		{
			m_WorkingThread.join();
		}
	}
}

void FileWatcher::RunInternal()
{
	while (m_IsRunning.load())
	{
		RemovedErasedFiles();
		CheckForNewFiles();

		std::this_thread::sleep_for(m_DelayBetweenChecks);
	}
}

void FileWatcher::Stop() noexcept
{
	bool expected = true;
	m_IsRunning.compare_exchange_strong(expected, false);
}

void FileWatcher::CheckForNewFiles()
{
	for (auto& file : std::filesystem::recursive_directory_iterator(m_FolderToWatch))
	{
		const std::string fileName = file.path().string();
		if (std::ranges::find(m_Files, fileName) != m_Files.end())
		{
			continue;
		}

		m_Files.emplace_back(fileName);
		m_OnFileCreatedCallback(fileName);
	}
}

void FileWatcher::RemovedErasedFiles()
{
	std::ranges::remove_if(m_Files,
		[](const std::string& file)
		{
			return !std::filesystem::exists(file);
		}
	);
}