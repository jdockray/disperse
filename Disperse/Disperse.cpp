
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
		std::vector<std::unique_ptr<CSVInput>> gridCSVInputs;
		std::vector<std::reference_wrapper<AbstractInput>> gridInputs;
		std::optional<std::string> gridInputFileNamesCommaDelimited = cmdLineArgs.getSingleArgumentOption('m');
		if (gridInputFileNamesCommaDelimited) {
			std::vector<std::string> gridInputFiles = getDelimitedElements(gridInputFileNamesCommaDelimited.value());
			gridCSVInputs.reserve(gridInputFiles.size()); // To prevent relocation as elements are added
			for (const std::string& gridInputFile : gridInputFiles) {
				gridCSVInputs.push_back(std::make_unique<CSVInput>(gridInputFile));
				gridInputs.push_back(*gridCSVInputs.back()); // Relies on no relocation of gridCSVInputs
			}
		}

		std::vector<std::unique_ptr<CSVInput>> listCSVInputs;
		std::vector<std::reference_wrapper<AbstractInput>> listInputs;
		std::optional<std::string> listInputFileNamesCommaDelimited = cmdLineArgs.getSingleArgumentOption('l');
		if (listInputFileNamesCommaDelimited) {
			std::vector<std::string> listInputFiles = getDelimitedElements(listInputFileNamesCommaDelimited.value());
			listCSVInputs.reserve(listInputFiles.size()); // To prevent relocation as elements are added
			for (const std::string& listInputFile : listInputFiles) {
				listCSVInputs.push_back(std::make_unique<CSVInput>(listInputFile));
				listInputs.push_back(*listCSVInputs.back()); // Relies on no relocation of listCSVInputs
			}
		}

		const std::string default_market_risk_name = "Market";
		std::optional<std::string> gridOutputFileName = cmdLineArgs.getSingleArgumentOption('r');
		std::unique_ptr<CSVOutput> gridOutput;
		if (gridOutputFileName) {
			gridOutput = std::make_unique<CSVOutput>(gridOutputFileName.value());
		}
		std::optional<std::string> listOutputFileName = cmdLineArgs.getSingleArgumentOption('i');
		std::unique_ptr<CSVOutput> listOutput;
		if (listOutputFileName) {
			listOutput = std::make_unique<CSVOutput>(listOutputFileName.value());
		}
		double additionalMarketRisk = CouldNotParseNumberException::convert(cmdLineArgs.getSingleArgumentOption('a').value_or("0"));
		runCombineCommand(gridInputs, listInputs, additionalMarketRisk, cmdLineArgs.getSingleArgumentOption('b').value_or(default_market_risk_name),
			gridOutput.get(), listOutput.get());
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
		std::vector<std::unique_ptr<CSVInput>> gridCSVInputs;
		std::vector<std::reference_wrapper<AbstractInput>> gridInputs;
		std::optional<std::string> gridInputFileNamesCommaDelimited = cmdLineArgs.getSingleArgumentOption('m');
		if (gridInputFileNamesCommaDelimited) {
			std::vector<std::string> gridInputFiles = getDelimitedElements(gridInputFileNamesCommaDelimited.value());
			gridCSVInputs.reserve(gridInputFiles.size()); // To prevent relocation as elements are added
			for (const std::string& gridInputFile : gridInputFiles) {
				gridCSVInputs.push_back(std::make_unique<CSVInput>(gridInputFile));
				gridInputs.push_back(*gridCSVInputs.back()); // Relies on no relocation of gridCSVInputs
			}
		}

		std::vector<std::unique_ptr<CSVInput>> listCSVInputs;
		std::vector<std::reference_wrapper<AbstractInput>> listInputs;
		std::optional<std::string> listInputFileNamesCommaDelimited = cmdLineArgs.getSingleArgumentOption('l');
		if (listInputFileNamesCommaDelimited) {
			std::vector<std::string> listInputFiles = getDelimitedElements(listInputFileNamesCommaDelimited.value());
			listCSVInputs.reserve(listInputFiles.size()); // To prevent relocation as elements are added
			for (const std::string& listInputFile : listInputFiles) {
				listCSVInputs.push_back(std::make_unique<CSVInput>(listInputFile));
				listInputs.push_back(*listCSVInputs.back()); // Relies on no relocation of listCSVInputs
			}
		}
		std::optional<std::string> gridOutputFileName = cmdLineArgs.getSingleArgumentOption('r');
		std::unique_ptr<CSVOutput> gridOutput;
		if (gridOutputFileName) {
			gridOutput = std::make_unique<CSVOutput>(gridOutputFileName.value());
		}
		std::optional<std::string> listOutputFileName = cmdLineArgs.getSingleArgumentOption('i');
		std::unique_ptr<CSVOutput> listOutput;
		if (listOutputFileName) {
			listOutput = std::make_unique<CSVOutput>(listOutputFileName.value());
		}
		double scalarToMultiplyBy = CouldNotParseNumberException::convert(cmdLineArgs.getSingleArgumentOption('s').value_or("1"));
		runMultiplyCommand(gridInputs, listInputs, scalarToMultiplyBy, gridOutput.get(), listOutput.get());
	}
	else if (command == "optimise")
	{
		const std::optional<std::string> factorGridFileName = cmdLineArgs.getSingleArgumentOption('m');
		const std::optional<std::string> factorListFileName = cmdLineArgs.getSingleArgumentOption('l');
		const std::optional<std::string> factorOutputFileName = cmdLineArgs.getSingleArgumentOption('f');
		const std::optional<std::string> groupInputFileName = cmdLineArgs.getSingleArgumentOption('g');
		const std::optional<std::string> groupOutputFileName = cmdLineArgs.getSingleArgumentOption('p');
		const std::list<std::string>& requiredArgs = cmdLineArgs.remainingArguments();
		std::unique_ptr<CSVInput> factorGridInput;
		if (factorGridFileName) {
			factorGridInput = std::make_unique<CSVInput>(factorGridFileName.value());
		}
		std::unique_ptr<CSVInput> factorListInput;
		if (factorListFileName) {
			factorListInput = std::make_unique<CSVInput>(factorListFileName.value());
		}
		std::unique_ptr<CSVOutput> factorOutput;
		if (factorOutputFileName) {
			factorOutput = std::make_unique<CSVOutput>(factorOutputFileName.value());
		}
		std::unique_ptr<CSVInput> groupInput;
		if (groupInputFileName) {
			groupInput = std::make_unique<CSVInput>(groupInputFileName.value());
		}
		std::unique_ptr<CSVOutput> groupOutput;
		if (groupOutputFileName) {
			groupOutput = std::make_unique<CSVOutput>(groupOutputFileName.value());
		}
		MissingArgumentException::verifyListLengthSufficient(requiredArgs, 3, "There are not enough positional command line arguments.");
		std::list<std::string>::const_iterator position = requiredArgs.begin();
		CSVInput input(*position);
		CSVOutput securityOutput = CSVOutput(*(++position));
		double minimumReturn = CouldNotParseNumberException::convert(*(++position));

		runOptimiseCommand(input, securityOutput, minimumReturn, factorGridInput.get(), factorListInput.get(), factorOutput.get(),
			groupInput.get(), groupOutput.get());
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
