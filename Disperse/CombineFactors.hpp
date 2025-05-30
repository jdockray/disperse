

#ifndef COMBINE_FACTORS
#define COMBINE_FACTORS

#include "CSVOutput.hpp"

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

void runCombineCommand(const std::optional<std::string>& gridInputFileNameString, const std::optional<std::string>& listInputFileNameString,
	const std::optional<std::string>& marketRiskString, const std::string& marketRiskName, AbstractCSVOutput* gridOutput,
	AbstractCSVOutput* listOutput);

void runMultiplyCommand(const std::optional<std::string>& gridInputFileNameString, const std::optional<std::string>& listInputFileNameString,
	const std::optional<std::string>& scalarToMultiplyBy, AbstractCSVOutput* gridOutput, AbstractCSVOutput* listOutput);

#endif // #ifndef COMBINE_FACTORS
