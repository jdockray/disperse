
#ifndef DISPERSE_INPUT
#define DISPERSE_INPUT

#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

ListOfSecurities inputSecurities(const std::string& inputFileName);
void inputFactorGrid(const std::string& inputFileName, ListOfSecurities& securities);
void inputFactorList(const std::string& inputFileName, ListOfSecurities& securities);
ListOfGroups inputGroups(const std::string& groupInputFileName);

#endif // #ifndef DISPERSE_INPUT

