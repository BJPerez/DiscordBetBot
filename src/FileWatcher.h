#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>

class FileWatcher
{
	using Callback = std::function<void(const std::filesystem::path&)>;

public:
	FileWatcher(std::string pathToFolder, const std::chrono::minutes delay, Callback onFileCreatedCallback) noexcept;
	FileWatcher(const FileWatcher& other) = delete;
	FileWatcher(FileWatcher&& other) noexcept;
	~FileWatcher();

	FileWatcher& operator=(const FileWatcher& other) = delete;
	FileWatcher& operator=(FileWatcher&& other) noexcept;

private:
	std::string m_FolderToWatch;
	std::chrono::minutes m_DelayBetweenChecks;
	Callback m_OnFileCreatedCallback;

	mutable std::condition_variable m_Notifier;
	mutable std::mutex m_Mutex;
	bool m_IsRunning{ true };
	std::thread m_WorkingThread;

	void CheckForFiles() const;
	void Run() const;
	bool Wait() const;
};