
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "Input.hpp"

void runOptimisation(
	const ListOfSecurities& securities,
	IOutput& securityOutput,
	double minimumReturn,
	IOutput* factorOutput = nullptr,
	IInput* groupInput = nullptr,
	IOutput* groupOutput = nullptr
);

#endif // DISPERSE_ELEMENTS
