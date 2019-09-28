#ifndef SIGNATUREGENERATOR_H
#define SIGNATUREGENERATOR_H

#include "ThreadManager.h"
#include "Reader.h"
#include "Writer.h"
class SignatureGenerator
{
public:
	SignatureGenerator() = delete;
	SignatureGenerator(const std::string& inFile, const std::string& outFile, unsigned int blockSize = 1);
	bool openFiles();
	void calcSignature();

private:
	Reader mFileReader;
	Writer mFileWriter;
	ThreadManager mThreadPool;
};
#endif // SIGNATUREGENERATOR_H
