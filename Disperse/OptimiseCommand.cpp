
#include "Optimise.hpp"
#include "OptimiseCommand.hpp"

void runOptimiseCommand(
	const std::string& securityInputFile,
	const std::string& securityOutputFile,
	double minimumReturn,
	const std::optional<std::string>& factorGridInputFile,
	const std::optional<std::string>& factorListInputFile,
	const std::optional<std::string>& factorOutputFile,
	const std::optional<std::string>& groupInputFile,
	const std::optional<std::string>& groupOutputFile
)
{
	CSVInput securityInput(securityInputFile);
	CSVOutput securityOutput = CSVOutput(securityOutputFile);
	std::unique_ptr<CSVInput> factorGridInput;
	if (factorGridInputFile) {
		factorGridInput = std::make_unique<CSVInput>(factorGridInputFile.value());
	}
	std::unique_ptr<CSVInput> factorListInput;
	if (factorListInputFile) {
		factorListInput = std::make_unique<CSVInput>(factorListInputFile.value());
	}
	std::unique_ptr<CSVOutput> factorOutput;
	if (factorOutputFile) {
		factorOutput = std::make_unique<CSVOutput>(factorOutputFile.value());
	}
	std::unique_ptr<CSVInput> groupInput;
	if (groupInputFile) {
		groupInput = std::make_unique<CSVInput>(groupInputFile.value());
	}
	std::unique_ptr<CSVOutput> groupOutput;
	if (groupOutputFile) {
		groupOutput = std::make_unique<CSVOutput>(groupOutputFile.value());
	}	
	runOptimisation(securityInput, securityOutput, minimumReturn, factorGridInput.get(), factorListInput.get(), factorOutput.get(),
		groupInput.get(), groupOutput.get());
}
