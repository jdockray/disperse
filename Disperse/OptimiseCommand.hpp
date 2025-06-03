
#ifndef DISPERSE_OPTIMISE_COMMAND
#define DISPERSE_OPTIMISE_COMMAND

#include "Optimise.hpp"

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

const std::string asset_title = "Asset";
const std::string allocation_title = "Allocation";
const std::string factor_title = "Factor";
const std::string exposure_title = "Exposure";
const std::string group_title = "Group";
const std::string proportion_title = "Proportion";

double runOptimiseCommand(
	IOptimisationCode& optimisationCode,
	const std::string& securityInputFile,
	const std::string& securityOutputFile,
	double minimumReturn,
	const std::optional<std::string>& factorGridInputFile = std::optional<std::string>(),
	const std::optional<std::string>& factorListInputFile = std::optional<std::string>(),
	const std::optional<std::string>& factorOutputFile = std::optional<std::string>(),
	const std::optional<std::string>& groupInputFile = std::optional<std::string>(),
	const std::optional<std::string>& groupOutputFile = std::optional<std::string>()
);

#endif // DISPERSE_OPTIMISE_COMMAND
