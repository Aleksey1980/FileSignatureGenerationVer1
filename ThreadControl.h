#ifndef THREADCONTROL_H
#define THREADCONTROL_H

#include <mutex>
#include <condition_variable>

class ThreadControl
{
public:
	ThreadControl(int count = 0);
	void post();
	void wait();

private:
	std::mutex mMutex;
	std::condition_variable mCv;
	int mCount;
};

#endif // THREADCONTROL_H
