#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <future>
#include "ThreadControl.h"
#include "Reader.h"

class Writer
{
public:
	Writer() = delete;
	Writer(const std::string& outFile, Reader& ReaderPtr);
	~Writer();
	bool openFile();
	void start();
	void stop();
	void post();
	void finish();
	void pushFutureInQueue(std::future<size_t>&& future);
	bool isFinished();

private:
	std::ofstream mFout;
	std::string mFileName;
	ThreadControl mSem;
	std::thread mThread;
	std::mutex mMutex;
	std::atomic<bool> mStopFlag;
	std::queue<std::future<size_t>> mFutureHashQueue;
	Reader& mReaderPtr;
};
#endif // FILEWRITER_H
