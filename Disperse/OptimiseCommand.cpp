
#include "Optimise.hpp"
#include "OptimiseCommand.hpp"
#include "Output.hpp"

double runOptimiseCommand(
	IOptimisationCode& optimisationCode,
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
	ListOfGroups listOfGroups;
	if (groupInputFile) {
		CSVInput groupInput = groupInputFile.value();
		listOfGroups = inputGroups(groupInput);
	}
	std::unique_ptr<CSVOutput> groupOutput;
	if (groupOutputFile) {
		groupOutput = std::make_unique<CSVOutput>(groupOutputFile.value());
	}	
	OptimisationResult result = optimisationCode.runOptimisation(securityListBuilder.getSecurityList(), minimumReturn, listOfGroups);
	CSVOutput securityOutput(securityOutputFile);
	AllocationOutputWriter(securityOutput).outputAllocations(securityListBuilder.getSecurityList().getIdentifiers(), result.allocations);
	if (factorOutputFile) {
		CSVOutput output(factorOutputFile.value());
		FactorExposureWriter(output).outputFactorExposures(result.factorNames, result.resultingFactorExposures);
	}
	if (groupOutputFile) {
		CSVOutput output(groupOutputFile.value());
		GroupProportionWriter(output).outputGroupProportions(listOfGroups.getIdentifiers(), result.proportionsInEachGroup);
	}
	return result.resultingRisk;
}
