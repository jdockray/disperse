
#include "ArgumentUnpacking.hpp"
#include "CmdLine.hpp"
#include "CombineFactorCommands.hpp"
#include "OptimiseCommand.hpp"
#include "ExpectedException.hpp"
#include "Help.hpp"

void callCombineCommand(const std::vector<std::string>& commandArgs)
{
	const std::string default_market_risk_name = "Market";
	CmdLineArgs cmdLineArgs(commandArgs);
	std::vector<std::string> gridInputFiles = getDelimitedElements(cmdLineArgs.getSingleArgumentOption('m').value_or(""));
	std::vector<std::string> listInputFiles = getDelimitedElements(cmdLineArgs.getSingleArgumentOption('l').value_or(""));
	double additionalMarketRisk = CouldNotParseNumberException::convert(cmdLineArgs.getSingleArgumentOption('a').value_or("0"));
	std::string marketRiskName = cmdLineArgs.getSingleArgumentOption('b').value_or(default_market_risk_name);
	std::optional<std::string> gridOutputFileName = cmdLineArgs.getSingleArgumentOption('r');
	std::optional<std::string> listOutputFileName = cmdLineArgs.getSingleArgumentOption('i');
	runCombineCommand(gridInputFiles, listInputFiles, additionalMarketRisk, marketRiskName,	gridOutputFileName, listOutputFileName);
}

void callHelpCommand(const std::vector<std::string>& commandArgs)
{
	CmdLineArgs cmdLineArgs(commandArgs);
	if (cmdLineArgs.remainingArguments().empty())
	{
		printGeneralHelp(std::cout);
	}
	else
	{
		printHelpForCommand(cmdLineArgs.remainingArguments().front(), std::cout);
	}
}

void callMultiplyCommand(const std::vector<std::string>& commandArgs)
{
	CmdLineArgs cmdLineArgs(commandArgs);
	std::vector<std::string> gridInputFiles = getDelimitedElements(cmdLineArgs.getSingleArgumentOption('m').value_or(""));
	std::vector<std::string> listInputFiles = getDelimitedElements(cmdLineArgs.getSingleArgumentOption('l').value_or(""));
	double scalarToMultiplyBy = CouldNotParseNumberException::convert(cmdLineArgs.getSingleArgumentOption('s').value_or("1"));
	std::optional<std::string> gridOutputFileName = cmdLineArgs.getSingleArgumentOption('r');
	std::optional<std::string> listOutputFileName = cmdLineArgs.getSingleArgumentOption('i');
	runMultiplyCommand(gridInputFiles, listInputFiles, scalarToMultiplyBy, gridOutputFileName, listOutputFileName);
}

void callOptimiseCommand(const std::vector<std::string>& commandArgs)
{
	CmdLineArgs cmdLineArgs(commandArgs);
	const std::optional<std::string> factorGridInput = cmdLineArgs.getSingleArgumentOption('m');
	const std::optional<std::string> factorListInput = cmdLineArgs.getSingleArgumentOption('l');
	const std::optional<std::string> factorOutputFile = cmdLineArgs.getSingleArgumentOption('f');
	const std::optional<std::string> groupInputFile = cmdLineArgs.getSingleArgumentOption('g');
	const std::optional<std::string> groupOutputFile = cmdLineArgs.getSingleArgumentOption('p');
	const std::list<std::string>& requiredArgs = cmdLineArgs.remainingArguments();
	MissingArgumentException::verifyListLengthSufficient(requiredArgs, 3, "There are not enough positional command line arguments.");
	std::list<std::string>::const_iterator position = requiredArgs.begin();
	const std::string& securityInputFileName = *position;
	const std::string& securityOutputFileName = *(++position);
	double minimumReturn = CouldNotParseNumberException::convert(*(++position));
	runOptimiseCommand(securityInputFileName, securityOutputFileName, minimumReturn, factorGridInput, factorListInput, factorOutputFile,
		groupInputFile, groupOutputFile);
}
