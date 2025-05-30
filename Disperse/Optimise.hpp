
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

void runOptimiseCommand(
	const std::string& inputFileName,
	const std::string& securityOutputFileName,
	const double minimumReturn,
	const std::optional<std::string> factorGridFileName = std::optional<std::string>(),
	const std::optional<std::string> factorListFileName = std::optional<std::string>(),
	const std::optional<std::string> factorOutputFileName = std::optional<std::string>(),
	const std::optional<std::string> groupInputFileName = std::optional<std::string>(),
	const std::optional<std::string> groupOutputFileName = std::optional<std::string>()
);

#endif // DISPERSE_ELEMENTS
