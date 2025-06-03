
#include "CmdLine.hpp"
#include "Exceptions.hpp"

CmdLineArgs::CmdLineArgs(const std::vector<std::string>& args)
    : arguments(args.begin(), args.end()) {
}

bool CmdLineArgs::getNoArgumentOption(const char option) {
    return findAndExcise(option, 0).has_value();
}

std::optional<std::string> CmdLineArgs::getSingleArgumentOption(const char option) {
    std::optional<std::vector<std::string>> result = findAndExcise(option, 1);
    return result.has_value() ? result.value().front() : std::optional<std::string>();
}

std::optional<std::vector<std::string>> CmdLineArgs::getMultipleArgumentOption(const char option, const unsigned int length) {
    return findAndExcise(option, length);
}

const std::list<std::string>& CmdLineArgs::remainingArguments() const {
    return arguments;
}

std::optional<std::vector<std::string>> CmdLineArgs::findAndExcise(const char option, const unsigned int length) {
    std::list<std::string>::const_iterator start = arguments.begin();
    const char optionCString[]{ '-', option, '\0' };
    while (start != arguments.end() && *start != optionCString) {
        ++start;
    }
    if (start == arguments.end()) {
        return {};
    }
    std::list<std::string>::const_iterator end = start;
    int elementCount = -1;
    do {
        ++elementCount;
        ++end;
    } while (elementCount < static_cast<int>(length) && end != arguments.end() && end->at(0) != '-');
    if (elementCount != static_cast<int>(length)) {
        throw MissingArgumentException("Wrong number of arguments for option " + option);
    }
    std::list<std::string>::const_iterator optionArgs = start;
    ++optionArgs;
    std::vector<std::string> excisedElements;
    if (elementCount > 0) {
        std::move(optionArgs, end, std::back_inserter(excisedElements));
    }
    arguments.erase(start, end);
    return std::optional(excisedElements);
}

