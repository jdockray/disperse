

#ifndef COMBINE_FACTORS
#define COMBINE_FACTORS

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

void runCombineCommand(const std::optional<std::string>& gridInputFileNameString, const std::optional<std::string>& listInputFileNameString,
	const std::optional<std::string>& marketRiskString, const std::string& marketRiskName, const std::optional<std::string>& gridOutputFileName,
	const std::optional<std::string>& listOutputFileName);

void runMultiplyCommand(const std::optional<std::string>& gridInputFileNameString, const std::optional<std::string>& listInputFileNameString,
	const std::optional<std::string>& scalarToMultiplyBy, const std::optional<std::string>& gridOutputFileName,
	const std::optional<std::string>& listOutputFileName);

#endif // #ifndef COMBINE_FACTORS
