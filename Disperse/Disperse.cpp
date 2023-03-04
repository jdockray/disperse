
#include "ExpectedException.hpp"
#include "Optimise.hpp"

#pragma warning(push, 0)
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#pragma warning(pop)

void runCommand(const int argc, const char* const argv[])
{
	MissingArgumentException::verifyTrue(argc > 1, "You must specify a command.");
	const std::string command(argv[1]);
	std::vector<std::string> commandArgs;
	commandArgs.reserve(argc - 2);
	for (int i = 2; i < argc; ++i)
	{
		commandArgs.push_back(std::string(argv[i]));
	}
	if (command == "optimise")
	{
		runOptimiseCommand(commandArgs);
	}
	else
	{
		throw InvalidCommandException("The command " + command + " is not recognised.");
	}
}

void runCatchingGlobalExceptions(const int argc, const char* const argv[])
{
	try
	{
		runCommand(argc, argv);
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
