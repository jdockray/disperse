
#ifndef DISPERSE_OUTPUT
#define DISPERSE_OUTPUT

#include <vector>
#include "Security.hpp"

void outputAllocations(const std::vector<Security>& securities, const std::vector<double>& allocations, const std::string& outputFileName);

void outputAllocations(const std::vector<Security>& securities, const std::vector<double>& allocations, std::ofstream& outputFile);

#endif // #ifndef DISPERSE_OUTPUT
