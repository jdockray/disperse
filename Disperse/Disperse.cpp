
#include "ArgumentUnpacking.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <vector>
#include <string>
#pragma warning(pop)

void runCommand(const int argc, const char* const argv[])
{
	MissingArgumentException::verifyTrue(argc > 1, "You must specify a command. Type 'Disperse.exe help' for usage.");
	const std::string command(argv[1]);
	std::vector<std::string> commandArgs;
	commandArgs.reserve(argc - 2);
	for (int i = 2; i < argc; ++i) // i signed to match argc
	{
		commandArgs.push_back(std::string(argv[i]));
	}
	if (command == "combine")
	{
		callCombineCommand(commandArgs);
	}
	else if (command == "help")
	{	
		callHelpCommand(commandArgs);
	}
	else if (command == "multiply")
	{
		callMultiplyCommand(commandArgs);
	}
	else if (command == "optimise")
	{
		callOptimiseCommand(commandArgs);
	}
	else
	{
		throw InvalidCommandException("The command " + command + " is not recognised. Type 'Disperse.exe help' for usage.");
	}
}

void runCatchingGlobalExceptions(const int argc, const char* const argv[])
{
	try
	{
		runCommand(argc, argv);
	}
	catch (const std::bad_alloc&)
	{
		throw InsufficientMemoryException();
	}
	catch (const csvstream_exception& ex) {
		throw CSVStreamException(ex);
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
	catch (const ExpectedException& ex)
	{
		std::cerr << ex.errorMessage;
		returnCode = ex.returnCode;
	}
#ifndef _DEBUG
	catch (const std::exception&)
	{
		UnexpectedException unexpectedException;
		std::cerr << unexpectedException.errorMessage;
	}
#endif
	return static_cast<int>(returnCode);
}
