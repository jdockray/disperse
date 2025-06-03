
#ifndef DISPERSE_COMBINE_FACTOR_COMMANDS
#define DISPERSE_COMBINE_FACTOR_COMMANDS

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

std::vector<std::string> getDelimitedElements(const std::string& delimitedList);

void runCombineCommand(
    const std::vector<std::string>& gridInputFiles,
    const std::vector<std::string>& listInputFiles,
    double additionalMarketRisk,
    const std::string& marketRiskName,
    const std::optional<std::string>& gridOutputFile,
    const std::optional<std::string>& listOutputFile
);

void runMultiplyCommand(
    const std::vector<std::string>& gridInputFiles,
    const std::vector<std::string>& listInputFiles,
    double scalarToMultiplyBy,
    const std::optional<std::string>& gridOutputFile,
    const std::optional<std::string>& listOutputFile
);

#endif // #ifndef DISPERSE_COMBINE_FACTOR_COMMANDS
