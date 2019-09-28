// FileSignatureGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SignatureGenerator.h"
#include <string>
#include <fstream>
#include <array>
#include <cmath>
#include <cstdlib>
using namespace std;

int main()
{
	string inputfilename;
	string outputfilename;
	string path;
	size_t  sizeint;
	std::cout << "Input path: ";
	std::cin >> path;
	std::cout << "Input inputfile name: ";
	std::cin >> inputfilename;
	std::cout << "Input outputfile name: ";
	std::cin >> outputfilename;
	std::cout << "Input size: ";
	std::cin >> sizeint;
	path = path + "\\";
	auto sizestr = std::to_string(sizeint);
	std::cout << "Full path: " << path + inputfilename << "\t your size: " << sizestr << std::endl;




	std::string inputFileName;
	std::string outputFileName;
	unsigned int blockSize = static_cast<unsigned int>(sizeint);
		try
		{
			if (blockSize < 1)
			{
				std::cout << "Error: allowed block size is 1 Mb minimum" << std::endl;
				return -1;
			}
			blockSize = static_cast<unsigned int>(sizeint);
			if (blockSize >= AVAILABLE_MEMORY / MEGABYTE_SIZE)
			{
				std::cout << "Error: block size is too big. Try smaller than 1024 Mb" << std::endl;
				return -1;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "Error: block size exception: " << e.what() << std::endl;
			return -1;
		}
	inputFileName = path + inputfilename;
	outputFileName = path + outputfilename;
	SignatureGenerator proc(inputFileName, outputFileName, blockSize);
	if (!proc.openFiles())
	{
		std::cout << "Error: bad input or output file" << std::endl;
		return -1;
	}
	else
	{
		proc.calcSignature();
	}
	std::cout << "\n-------FINISH----------\n" << std::endl;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
