#include "Writer.h"

Writer::Writer(const std::string& outFile, Reader& ReaderPtr) :
	mFileName(outFile),
	mSem(0),
	mStopFlag(false),
	mReaderPtr(ReaderPtr)
{		
}

Writer::~Writer()
{
	mStopFlag = true;
	if (mThread.joinable())
	{
		mSem.post();
		mThread.join();
	}
	if (mFout.is_open())
	{
		mFout.close();
	}
}

bool Writer::openFile()
{
	try
	{
		mFout.open(mFileName, std::ios::out | std::ios::binary);
		return mFout.is_open();
	}
	catch (const std::exception & e)
	{
		mStopFlag = true;
		std::cout << "Error: open file exception: " << e.what() << std::endl;
		return false;
	}
}

void Writer::start()
{
	auto write = [this]()
	{
		try
		{
			while (true)
			{
				if (mStopFlag)
				{
					break;
				}
				if (!mFutureHashQueue.empty())
				{
					size_t hash = 0;
					{
						std::lock_guard<std::mutex> lock(mMutex);
						hash = mFutureHashQueue.front().get();
						mFutureHashQueue.pop();
					}
					mFout << hash;
					mReaderPtr.post();
					std::cout << hash << std::endl;
				}
				mSem.wait();
			}
		}
		catch (const std::exception& e)
		{
			mStopFlag = true;
			std::cout << "\nWriter write() exception caught: " << e.what() << std::endl;
			//std::exit(EXIT_FAILURE);
			mThread.join();
			throw;
		}
	};
	mThread = std::thread(write);
}

void Writer::stop()
{
	mStopFlag = true;
	if (mThread.joinable())
	{
		mSem.post();
		mThread.join();
	}
}

void Writer::post()
{
	mSem.post();
}

void Writer::finish()
{
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (mFutureHashQueue.empty())
		{
			mStopFlag = true;
		}
	}
	mSem.post();
	mThread.join();
}

void Writer::pushFutureInQueue(std::future<size_t>&& future)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mFutureHashQueue.push(std::move(future));
}

bool Writer::isFinished()
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mFutureHashQueue.empty();
}