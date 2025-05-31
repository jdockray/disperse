
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
	SecurityListBuilder securityListBuilder(securityInput);
	CSVOutput securityOutput = CSVOutput(securityOutputFile);
	if (factorGridInputFile)
	{
		CSVInput factorGridInput(factorGridInputFile.value());
		securityListBuilder.loadFactorsFromGrid(factorGridInput);
	}
	if (factorListInputFile)
	{
		CSVInput factorListInput(factorListInputFile.value());
		securityListBuilder.loadFactorsFromList(factorListInput);
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
	runOptimisation(securityListBuilder.getSecurityList(), securityOutput, minimumReturn, factorOutput.get(),
		groupInput.get(), groupOutput.get());
}
