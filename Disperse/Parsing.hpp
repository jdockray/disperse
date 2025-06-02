
#ifndef DISPERSE_PARSING
#define DISPERSE_PARSING

#include "Exceptions.hpp"

#pragma warning(push, 0)
#include <string>
#pragma warning(pop)

double convertToNumber(const std::string& stringToParse, const std::string& context = "");

#endif