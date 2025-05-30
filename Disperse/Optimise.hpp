
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "CSVOutput.hpp"

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

void runOptimiseCommand(
	const std::string& inputFileName,
	AbstractCSVOutput& securityOutput,
	const double minimumReturn,
	const std::optional<std::string> factorGridFileName = std::optional<std::string>(),
	const std::optional<std::string> factorListFileName = std::optional<std::string>(),
	AbstractCSVOutput* factorOutput = nullptr,
	const std::optional<std::string> groupInputFileName = std::optional<std::string>(),
	AbstractCSVOutput* groupOutput = nullptr
);

#endif // DISPERSE_ELEMENTS
