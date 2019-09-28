#ifndef READER_H
#define READER_H

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include "ThreadControl.h"

static constexpr auto MEGABYTE_SIZE = 1024 * 1024;
static constexpr auto AVAILABLE_MEMORY = 1024 * MEGABYTE_SIZE;

class Reader
{
public:
	Reader() = delete;
	Reader(const std::string& inFile, unsigned int blockSize = 1);
	~Reader();
	bool openFile();
	void start();
	void stop();
	void post();
	void finish();
	bool isFinished();
	bool getDataBlock(std::string& dataBlock);

private:
	std::ifstream mFin;
	std::string mFileName;
	unsigned int mBlockSize;
	ThreadControl mSem;
	std::thread mThread;
	std::mutex mMutex;
	std::atomic<bool> mStopFlag;
	bool mIsFinised;
	std::queue<std::string> mDataBlockQueue;
};
#endif // READER_H
