
#ifndef DISPERSE_INPUT
#define DISPERSE_INPUT

#include "CSVInput.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

ListOfSecurities inputSecurities(AbstractInput& input);
void inputFactorGrid(AbstractInput& input, ListOfSecurities& securities);
void inputFactorList(AbstractInput& input, ListOfSecurities& securities);
ListOfGroups inputGroups(AbstractInput& input);

#endif // #ifndef DISPERSE_INPUT

