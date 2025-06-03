
#include "Parsing.hpp"

double convertToNumber(const std::string& stringToParse, const std::string& context) {
    try {
        return std::stof(stringToParse);
    }
    catch (const std::invalid_argument&) {
        throw CouldNotParseNumberException(stringToParse, context);
    }
    catch (const std::out_of_range&) {
        throw CouldNotParseNumberException(stringToParse, context);
    }
};
