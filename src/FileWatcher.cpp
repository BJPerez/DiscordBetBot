#include "FileWatcher.h"

#include <algorithm>

FileWatcher::FileWatcher(std::string pathToFolder, const std::chrono::minutes delay, Callback onFileCreatedCallback) noexcept:
	m_FolderToWatch(std::move(pathToFolder)), m_DelayBetweenChecks(delay), m_OnFileCreatedCallback(std::move(onFileCreatedCallback)),
	m_WorkingThread(std::thread(&FileWatcher::Run, this))
{
}

FileWatcher::FileWatcher(FileWatcher&& other) noexcept: m_FolderToWatch(std::move(other.m_FolderToWatch)), m_DelayBetweenChecks(other.m_DelayBetweenChecks),
	m_OnFileCreatedCallback(std::move(other.m_OnFileCreatedCallback)), m_WorkingThread(std::move(other.m_WorkingThread))
{
}

FileWatcher::~FileWatcher()
{
	{
		std::lock_guard lock{ m_Mutex };
		m_IsRunning = false;
	}

	m_Notifier.notify_one();
	m_WorkingThread.join();
}

FileWatcher& FileWatcher::operator=(FileWatcher&& other) noexcept
{
	m_FolderToWatch = std::move(other.m_FolderToWatch);
	m_DelayBetweenChecks = other.m_DelayBetweenChecks;
	m_OnFileCreatedCallback = std::move(other.m_OnFileCreatedCallback);
	m_WorkingThread = std::move(other.m_WorkingThread);

	return *this;
}

void FileWatcher::Run() const
{
	while (Wait())
	{
		CheckForFiles();
	}
}

bool FileWatcher::Wait() const
{
	std::unique_lock lock{ m_Mutex };
	m_Notifier.wait_for(lock, m_DelayBetweenChecks);
	return m_IsRunning;
}

void FileWatcher::CheckForFiles() const
{
	// Note that if the file hasn't been deleted by the callback, it will trigger the callback again
	for (auto& file : std::filesystem::recursive_directory_iterator(m_FolderToWatch))
	{
		m_OnFileCreatedCallback(file.path().string());
	}
}