#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <exception>
#include <iostream>

class ThreadManager
{
public:
	ThreadManager(size_t threadsNumber);
	~ThreadManager();
	std::future<size_t> processDataBlock(std::string&& input);
	ThreadManager() = delete;
	ThreadManager(const ThreadManager&) = delete;
	ThreadManager(ThreadManager&&) = delete;
	ThreadManager& operator=(const ThreadManager&) = delete;
	ThreadManager& operator=(ThreadManager&&) = delete;

private:
	std::vector<std::thread> mThreads;
	std::queue<std::function<void()>> mTasks;
	std::mutex mMutex;
	std::condition_variable mCv;
	std::atomic<bool> mStopFlag;
};
#endif // THREADMANAGER_H
