
#ifndef DISPERSE_OUTPUT
#define DISPERSE_OUTPUT

#include "Security.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

void outputAllocations(const std::vector<std::string>& securityIdentifiers, const std::vector<double>& allocations, const std::string& outputFileName);
void outputFactorExposures(const std::vector<std::string>& factorNames, const std::vector<double>& exposures, const std::string& outputFileName);

#endif // #ifndef DISPERSE_OUTPUT
