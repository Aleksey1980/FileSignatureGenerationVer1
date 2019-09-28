#include "SignatureGenerator.h"
SignatureGenerator::SignatureGenerator(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
	mFileReader(inFile, blockSize),
	mFileWriter(outFile, mFileReader),
	mThreadPool(std::thread::hardware_concurrency() - 1) // one thread is for work with files
{
}

bool SignatureGenerator::openFiles()
{
	return mFileReader.openFile() && mFileWriter.openFile();
}

void SignatureGenerator::calcSignature()
{
	mFileReader.start();
	mFileWriter.start();
	try
	{
		while (!mFileReader.isFinished() || !mFileWriter.isFinished())
		{
			std::string dataBlock;
			if (mFileReader.getDataBlock(dataBlock))
			{
				auto futureHash = mThreadPool.processDataBlock(std::move(dataBlock));
				mFileWriter.pushFutureInQueue(std::move(futureHash));
				mFileWriter.post();
			}
		}
		mFileWriter.finish();
		mFileReader.finish();
	}
	catch (const std::exception& e)
	{
		mFileReader.stop();
		mFileWriter.stop();
		std::cout << "\nSignatureGenerator calcSignature() exception caught: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}