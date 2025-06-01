
#include "Optimise.hpp"
#include "OptimiseCommand.hpp"
#include "Writers.hpp"

OSQPSolver osqpSolver;

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
) {
	CSVInput securityInput(securityInputFile);
	SecurityListBuilder securityListBuilder(securityInput);
	if (factorGridInputFile) {
		CSVInput input(factorGridInputFile.value());
		GridFileReader reader(input);
		securityListBuilder.loadFactors(reader);
	}
	if (factorListInputFile) {
		CSVInput input(factorListInputFile.value());
		ListFileReader reader(input);
		securityListBuilder.loadFactors(reader);
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
	OptimisationResult result = optimisationCode.runOptimisation(osqpSolver, securityListBuilder.getSecurityList(), minimumReturn, listOfGroups);
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
