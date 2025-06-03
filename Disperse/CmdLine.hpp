
#ifndef DISPERSE_COMMAND_LINE
#define DISPERSE_COMMAND_LINE

#pragma warning(push, 0)
#include <list>
#include <optional>
#include <string>
#include <vector>
#pragma warning(pop)

class CmdLineArgs {
public:
    CmdLineArgs(const std::vector<std::string>& args);
    bool getNoArgumentOption(const char option);
    std::optional<std::string> getSingleArgumentOption(const char option);
    std::optional<std::vector<std::string>> getMultipleArgumentOption(const char option, const unsigned int length);
    const std::list<std::string>& remainingArguments() const;

private:
    std::list<std::string> arguments;

    std::optional<std::vector<std::string>> findAndExcise(const char option, const unsigned int length);
};

#endif // #ifndef DISPERSE_COMMAND_LINE
