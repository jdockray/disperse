#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "ExpectedException.hpp"

void run(std::ifstream &inputFile, std::ofstream &outputFile)
{

}

void run(const std::string &inputFileName, const std::string &outputFileName, const std::string factorCombiningFormula)
{
	try
	{
		std::ifstream inputFile;
		inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			inputFile.open(inputFileName, std::ios_base::in);
		}
		catch (std::ios_base::failure)
		{
			throw InputFileException(inputFileName);
		}
		std::ofstream outputFile;
		outputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		try
		{
			outputFile.open(outputFileName, std::ios_base::out);
		}
		catch (std::ios_base::failure)
		{
			throw OutputFileException(outputFileName);
		}
		run(inputFile, outputFile);
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
	



	run(std::string(argv[0]), std::string(argv[1]), std::string(argv[2]));
}

int main(int argc, char* argv[])
{
	ReturnCode returnCode = ReturnCode::UNEXPECTED_EXCEPTION;
	try
	{
		run(argc, argv);
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
		std::cerr << "An unexpected error occurred.";
	}
#endif
	return static_cast<int>(returnCode);
}
