#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>
#include <vector>

class FileWatcher
{
	using Callback = std::function<void(const std::filesystem::path&)>;

public:
	FileWatcher(std::string pathToFolder, std::chrono::minutes delay, Callback onFileCreatedCallback) noexcept;
	FileWatcher(const FileWatcher& other) = delete;
	FileWatcher(FileWatcher&& other) noexcept;
	~FileWatcher();

	FileWatcher& operator=(const FileWatcher& other) = delete;
	FileWatcher& operator=(FileWatcher&& other) noexcept;

	void Run(const bool returnAfter = true);
	void Stop() noexcept;

private:
	std::string m_FolderToWatch;
	std::chrono::minutes m_DelayBetweenChecks;
	std::atomic<bool> m_IsRunning{ false };
	Callback m_OnFileCreatedCallback;
	std::thread m_WorkingThread;

	void CheckForFiles() const;
	void RunInternal() const;
};