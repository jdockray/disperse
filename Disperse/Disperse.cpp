#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "ExpectedException.hpp"
#include "CmdLine.hpp"
#include "Optimisation.hpp"

std::ifstream openInputFile(const std::string &fileName)
{
	std::ifstream inputFile;
	inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		inputFile.open(fileName, std::ios_base::in);
	}
	catch (std::ios_base::failure)
	{
		throw InputFileException(fileName);
	}
}


void run(std::string inputFileName, std::string& outputFileName, const double minimumReturn)
{
	try
	{
		run(inputFile, outputFile, minimumReturn);
		inputFile.close();
		outputFile.close();
	}
	catch (std::ios_base::failure)
	{
		throw IOException();
	}
}

void run(const unsigned int argc, const char* const argv[])
{
	CmdLineArgs cmdLineArgs(argc, argv);
	const std::list<std::string>& requiredArgs = cmdLineArgs.remainingArguments;
	MissingArgumentException::verifyListLengthSufficient(requiredArgs, 3);
	std::list<std::string>::const_iterator position = requiredArgs.begin();
	std::string inputFileName = *position;
	std::string outputFileName = *(++position);
	double minimumReturn = CouldNotParseNumberException::convert(*(++position));
	run(openInputFile(inputFileName), openOutputFile(outputFileName), minimumReturn);
}

void runCatchingGlobalExceptions(const unsigned int argc, const char* const argv[])
{
	try
	{
		run(argc, argv);
	}
	catch (std::bad_alloc)
	{
		throw InsufficientMemoryException();
	}
}

int main(int argc, char* argv[])
{
	ReturnCode returnCode = ReturnCode::UNEXPECTED_EXCEPTION;
	try
	{
		runCatchingGlobalExceptions(argc, argv);
		returnCode = ReturnCode::SUCCESS;
	}
	catch (ExpectedException ex)
	{
		std::cerr << ex.errorMessage;
		returnCode = ex.returnCode;
	}
#ifndef _DEBUG
	catch (...)
	{
		UnexpectedException unexpectedException;
		std::cerr << unexpectedException.errorMessage;
	}
#endif
	return static_cast<int>(returnCode);
}
