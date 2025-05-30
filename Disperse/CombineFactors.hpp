

#ifndef COMBINE_FACTORS
#define COMBINE_FACTORS

#include "CSVInput.hpp"
#include "CSVOutput.hpp"

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

std::vector<std::string> getDelimitedElements(const std::string& delimitedList);

void runCombineCommand(
	std::vector<std::reference_wrapper<AbstractInput>>& gridInputs,
	std::vector<std::reference_wrapper<AbstractInput>>& listInputs,
	double additionalMarketRisk,
	const std::string& marketRiskName,
	AbstractOutput* gridOutput,
	AbstractOutput* listOutput
);

void runMultiplyCommand(
	std::vector<std::reference_wrapper<AbstractInput>>& gridInputs,
	std::vector<std::reference_wrapper<AbstractInput>>& listInputs,
	double scalarToMultiplyBy,
	AbstractOutput* gridOutput,
	AbstractOutput* listOutput
);

#endif // #ifndef COMBINE_FACTORS
