
#include "Optimise.hpp"
#include "OptimiseCommand.hpp"
#include "Writers.hpp"

// This file is at the centre of my complex Disperse program.
// See the main readme file of this repository.

// I believe this file illustrates my software engineering experience.
// It orchestrates the main "optimise" operation performed by my software.
// It shows the concrete components being instantiated and injected into the components that depend on them.
// It illustrates how I have given classes a single responsibility.
// The operation is very complicated but encapsulation hides the complexity.

// Separating concerns is important as a piece of software gets larger.
// It allows individual parts to be modified or removed without breaking other functionality.
// It enables code to be used in new features or copied out and used in other software.
// Injecting dependencies also allows components to be swapped for dummy or mock objects in unit testing.

OSQPSolver osqpSolver;                                                                              // OSQPSolver implements ISolver

double runOptimiseCommand(
    IOptimisationCode& optimisationCode,

    const std::string& securityInputFile,           // Name of CSV file containing a list of securities with expected returns and volatilities
    const std::string& securityOutputFile,          // The place to write the output CSV containing the allocations from the optimisation.
    double minimumReturn,                           // The objective of the optimisation. This return must be achieved.

    // The amount each investment depends on each risk factor (to generate the covariance matrix) can be input in two ways: (One required)
    const std::optional<std::string>& factorGridInputFile,      // As an investment vs risk matrix
    const std::optional<std::string>& factorListInputFile,      // The same data as a list with rows: investment name, risk name, fraction

    const std::optional<std::string>& factorOutputFile,         // The risk exposures of the portfolio resulting from the optimisation.
    const std::optional<std::string>& groupInputFile,           // Constraints to limit allocations to certain investments or sets of investments
    const std::optional<std::string>& groupOutputFile           // The amount in each constrained group after the optimisation.
) {
    // Reading input files
    CSVInput securityInput(securityInputFile);                  // CSVInput implements IInput
    SecurityListBuilder securityListBuilder(securityInput);
    if (factorGridInputFile) {
        CSVInput input(factorGridInputFile.value());
        GridFileReader reader(input);                           // GridFileReader implements IReader
        securityListBuilder.loadFactors(reader);
    }
    if (factorListInputFile) {
        CSVInput input(factorListInputFile.value());
        ListFileReader reader(input);
        securityListBuilder.loadFactors(reader);
    }
    ListOfGroups listOfGroups;                      // ListOfGroups implements IdentifiedObjectList<Group> and ProportionLimitedObjectList<Group>
    if (groupInputFile) {
        CSVInput groupInput = groupInputFile.value();
        listOfGroups = inputGroups(groupInput);
    }

    // Processing                                               // OptimisationCode implements IOptimisationCode
    OptimisationResult result = optimisationCode.runOptimisation(osqpSolver, securityListBuilder.getSecurityList(), minimumReturn, listOfGroups);

    // Output
    CSVOutput securityOutput(securityOutputFile);                                                   // CSVOutput implements IOutput,    
    AllocationWriter(securityOutput, asset_title, allocation_title)                                 // AllocationWriter implements IWriter
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
