
#ifndef DISPERSE_OUTPUT
#define DISPERSE_OUTPUT

#include "CSVOutput.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

void outputAllocations(const std::vector<std::string>& securityIdentifiers, const std::vector<double>& allocations, AbstractOutput& csvOutput);
void outputFactorExposures(const std::vector<std::string>& factorNames, const std::vector<double>& exposures, AbstractOutput& csvOutput);
void outputGroupProportions(const std::vector<std::string> groupNames, const std::map<std::string, double>& groupProportions, AbstractOutput& csvOutput);

#endif // #ifndef DISPERSE_OUTPUT
