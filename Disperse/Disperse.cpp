
#include "CmdLine.hpp"
#include "CombineFactors.hpp"
#include "Optimise.hpp"
#include "ExpectedException.hpp"
#include "Help.hpp"

#pragma warning(push, 0)
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
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
	CmdLineArgs cmdLineArgs(commandArgs);
	if (command == "combine")
	{
		const std::string default_market_risk_name = "Market";
		runCombineCommand(cmdLineArgs.getSingleArgumentOption('m'), cmdLineArgs.getSingleArgumentOption('l'), cmdLineArgs.getSingleArgumentOption('a'),
			cmdLineArgs.getSingleArgumentOption('b').value_or(default_market_risk_name), cmdLineArgs.getSingleArgumentOption('r'),
			cmdLineArgs.getSingleArgumentOption('i'));
	}
	else if (command == "help")
	{	
		if (cmdLineArgs.remainingArguments().empty())
		{
			printGeneralHelp(std::cout);
		}
		else
		{
			printHelpForCommand(cmdLineArgs.remainingArguments().front(), std::cout);
		}
	}
	else if (command == "multiply")
	{
		runMultiplyCommand(cmdLineArgs.getSingleArgumentOption('m'), cmdLineArgs.getSingleArgumentOption('l'), cmdLineArgs.getSingleArgumentOption('s'),
			cmdLineArgs.getSingleArgumentOption('r'), cmdLineArgs.getSingleArgumentOption('i'));
	}
	else if (command == "optimise")
	{
		const std::optional<std::string> factorGridFileName = cmdLineArgs.getSingleArgumentOption('m');
		const std::optional<std::string> factorListFileName = cmdLineArgs.getSingleArgumentOption('l');
		const std::optional<std::string> factorOutputFileName = cmdLineArgs.getSingleArgumentOption('f');
		const std::optional<std::string> groupInputFileName = cmdLineArgs.getSingleArgumentOption('g');
		const std::optional<std::string> groupOutputFileName = cmdLineArgs.getSingleArgumentOption('p');
		const std::list<std::string>& requiredArgs = cmdLineArgs.remainingArguments();
		MissingArgumentException::verifyListLengthSufficient(requiredArgs, 3, "There are not enough positional command line arguments.");
		std::list<std::string>::const_iterator position = requiredArgs.begin();
		std::string inputFileName = *position;
		std::string securityOutputFileName = *(++position);
		double minimumReturn = CouldNotParseNumberException::convert(*(++position));
		runOptimiseCommand(inputFileName, securityOutputFileName, minimumReturn, factorGridFileName, factorListFileName,
			factorOutputFileName, groupInputFileName, groupOutputFileName);
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
