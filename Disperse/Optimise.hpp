
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "Input.hpp"

void runOptimisation(
	const ListOfSecurities& securities,
	AbstractOutput& securityOutput,
	double minimumReturn,
	AbstractOutput* factorOutput = nullptr,
	AbstractInput* groupInput = nullptr,
	AbstractOutput* groupOutput = nullptr
);

#endif // DISPERSE_ELEMENTS
