
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "CSVInput.hpp"
#include "CSVOutput.hpp"

void runOptimisation(
	AbstractInput& securityInput,
	AbstractOutput& securityOutput,
	double minimumReturn,
	AbstractInput* factorGridInput = nullptr,
	AbstractInput* factorListInput = nullptr,
	AbstractOutput* factorOutput = nullptr,
	AbstractInput* groupInput = nullptr,
	AbstractOutput* groupOutput = nullptr
);

#endif // DISPERSE_ELEMENTS
