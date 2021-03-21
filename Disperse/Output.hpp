
#ifndef DISPERSE_OUTPUT
#define DISPERSE_OUTPUT

#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

void outputAllocations(const std::vector<std::string>& securityIdentifiers, const std::vector<double>& allocations, const std::string& outputFileName);
void outputFactorExposures(const std::vector<std::string>& factorNames, const std::vector<double>& exposures, const std::string& outputFileName);
void outputGroupProportions(const std::vector<std::string> groupNames, const std::map<std::string, double>& groupProportions, const std::string& groupOutputFileName);

#endif // #ifndef DISPERSE_OUTPUT
