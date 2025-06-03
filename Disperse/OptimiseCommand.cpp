
#include "Optimise.hpp"
#include "OptimiseCommand.hpp"
#include "Writers.hpp"

OSQPSolver osqpSolver;												// OSQPSolver implements ISolver

double runOptimiseCommand(
	IOptimisationCode& optimisationCode,							// OptimisationCode implements IOptimisationCode
	const std::string& securityInputFile,
	const std::string& securityOutputFile,
	double minimumReturn,
	const std::optional<std::string>& factorGridInputFile,
	const std::optional<std::string>& factorListInputFile,
	const std::optional<std::string>& factorOutputFile,
	const std::optional<std::string>& groupInputFile,
	const std::optional<std::string>& groupOutputFile
) {
	// Input
	CSVInput securityInput(securityInputFile);						// CSVInput implements IInput
	SecurityListBuilder securityListBuilder(securityInput);
	if (factorGridInputFile) {
		CSVInput input(factorGridInputFile.value());
		GridFileReader reader(input);								// GridFileReader implements IReader
		securityListBuilder.loadFactors(reader);
	}
	if (factorListInputFile) {
		CSVInput input(factorListInputFile.value());
		ListFileReader reader(input);
		securityListBuilder.loadFactors(reader);
	}
	ListOfGroups listOfGroups;										// ListOfGroups implements IdentifiedObjectList<Group> and ProportionLimitedObjectList<Group>
	if (groupInputFile) {
		CSVInput groupInput = groupInputFile.value();
		listOfGroups = inputGroups(groupInput);
	}

	// Processing
	OptimisationResult result = optimisationCode.runOptimisation(osqpSolver, securityListBuilder.getSecurityList(), minimumReturn, listOfGroups);
	
	// Output
	CSVOutput securityOutput(securityOutputFile);													// CSVOutput implements IOutput,	
	AllocationWriter(securityOutput, asset_title, allocation_title)									// AllocationWriter implements IWriter
		.write(securityListBuilder.getSecurityList().getIdentifiers(), result.allocations);
	if (factorOutputFile) {
		CSVOutput factorOutput(factorOutputFile.value());
		AllocationWriter(factorOutput, factor_title, exposure_title).write(result.factorNames, result.resultingFactorExposures);
	}
	if (groupOutputFile) {
		CSVOutput groupOutput(groupOutputFile.value());
		AllocationWriter(groupOutput, group_title, proportion_title).write(listOfGroups.getIdentifiers(), result.proportionsInEachGroup);
	}
	return result.resultingRisk;
}
