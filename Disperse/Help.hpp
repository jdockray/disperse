
#ifndef DISPERSE_HELP
#define DISPERSE_HELP

#pragma warning(push, 0)
#include <ostream>
#include <vector>
#include <string>
#pragma warning(pop)

void printHelpForCommand(const std::string& commandForWhichHelpIsRequested, std::ostream& out);
void printGeneralHelp(std::ostream& out);

#endif // DISPERSE_HELP
