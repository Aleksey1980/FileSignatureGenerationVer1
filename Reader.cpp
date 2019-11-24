#include "Reader.h"

Reader::Reader(const std::string& inFile, unsigned int blockSize) :
	mFileName(inFile),
	mBlockSize(MEGABYTE_SIZE* blockSize),
	mSem((AVAILABLE_MEMORY / (MEGABYTE_SIZE * blockSize))),
	mStopFlag(false),
	mIsFinised(false)
{
}

Reader::~Reader()
{
	mStopFlag = true;
	if (mThread.joinable())
	{
		mSem.post();
		mThread.join();
	}
	if (mFin.is_open())
	{
		mFin.close();
	}
}

bool Reader::openFile()
{
	try
	{
		mFin.open(mFileName, std::ios::in | std::ios::binary);
		mFin.unsetf(std::ios::skipws);
		return mFin.is_open();
	}
	catch(const std::exception & e)
	{
		mStopFlag = true;
		std::cout << "Error: open file exception: " << e.what() << std::endl;
		return false;
	}
}

void Reader::start()
{
	auto read = [this]()
	{
		try
		{
			while (mFin)
			{
				if (mStopFlag)
				{
					break;
				}
				mSem.wait();
				std::vector<char> readData; //std::string readData;
				readData.assign(mBlockSize, 0);
				mFin.read(&readData.at(0), static_cast<int>(mBlockSize));
				{
					std::lock_guard<std::mutex> lock(mMutex);
					mDataBlockQueue.push(std::move(readData));
				}


			}
		}
		catch (const std::exception& e)
		{
			mStopFlag = true;
			std::cout << "\nReader read() exception caught: " << e.what() << std::endl;
			//std::exit(EXIT_FAILURE);
			mThread.join();
			throw;

		}
		mIsFinised = true;
	};
	mThread = std::thread(read);
}

void Reader::stop()
{
	mStopFlag = true;
	if (mThread.joinable())
	{
		mSem.post();
		mThread.join();
	}
}

void Reader::post()
{
	mSem.post();
}

void Reader::finish()
{
	mThread.join();
}

bool Reader::isFinished()
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mIsFinised && mDataBlockQueue.empty();
}

//bool Reader::getDataBlock(std::string& dataBlock)
bool Reader::getDataBlock(std::vector<char>& dataBlock)
{
	bool returnValue = false;
	try
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (!mDataBlockQueue.empty())
		{
			dataBlock = std::move(mDataBlockQueue.front());
			mDataBlockQueue.pop();
			returnValue = true;
		}
	}
	catch (const std::exception& e)
	{
		mStopFlag = true;
		mIsFinised = true;
		std::cout << "\nReader getDataBlock() exception caught: " << e.what() << std::endl;
		return returnValue;
	}
	return returnValue;
}