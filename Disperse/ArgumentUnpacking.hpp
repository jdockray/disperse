
#ifndef DISPERSE_ARGUMENT_UNPACKING
#define DISPERSE_ARGUMENT_UNPACKING

#pragma warning(push, 0)
#include <vector>
#include <string>
#pragma warning(pop)

void callCombineCommand(const std::vector<std::string>& commandArgs);
void callHelpCommand(const std::vector<std::string>& commandArgs);
void callMultiplyCommand(const std::vector<std::string>& commandArgs);
void callOptimiseCommand(const std::vector<std::string>& commandArgs);

#endif //  #ifndef DISPERSE_ARGUMENT_UNPACKING
