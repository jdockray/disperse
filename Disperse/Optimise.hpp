
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "CSVOutput.hpp"

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

void runOptimiseCommand(
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
