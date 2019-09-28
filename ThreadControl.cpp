#include "ThreadControl.h"

ThreadControl::ThreadControl(int count) :
	mCount(count)
{
}

void ThreadControl::post()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mCount++;
	mCv.notify_one();
}

void ThreadControl::wait()
{
	std::unique_lock<std::mutex> lock(mMutex);
	while (mCount == 0)
	{
		mCv.wait(lock);
	}
	mCount--;
}