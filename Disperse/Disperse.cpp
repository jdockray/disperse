#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "ExpectedException.hpp"
#include "CmdLine.hpp"
#include "Optimisation.hpp"

// The limit is the maximum allowed risk or minimum allowed return
void run(Objective objective, double limit, const std::string &inputFileName, const std::string &outputFileName,
	const std::string &factorCombinationFormula)
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
	CmdLineArgs cmdLineArgs(argc, argv);


	const std::list<std::string> &requiredArgs = cmdLineArgs.remainingArguments;
	MissingArgumentException::verifyListLengthSufficient(requiredArgs, 2);
	run(*requiredArgs.begin(), *std::next(requiredArgs.begin()));
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
