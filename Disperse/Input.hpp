
#ifndef DISPERSE_INPUT
#define DISPERSE_INPUT

#include "Security.hpp"
#include "Matrices.hpp"
#include "dlib\matrix.h"
#include <vector>

std::vector<Security>& inputSecurities(const std::string& inputFileName);
void augmentFactors(std::vector<Security>& ssecurities, const std::string& inputFileName);

#endif // #ifndef DISPERSE_INPUT
