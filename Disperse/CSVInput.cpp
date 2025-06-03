
#include "Input.hpp"

#pragma warning(push, 0)
#include <map>
#include <string>
#include <vector>
#pragma warning(pop)

CSVInput::CSVInput(const std::string& inputFileName)
    : inputFileName(inputFileName), inputStream(inputFileName) {
}

std::string CSVInput::getName() const {
    return inputFileName;
}

std::vector<std::string> CSVInput::getHeader() const {
    return inputStream.getheader();
}

std::map<std::string, std::string> CSVInput::readEntryAsMap() {
    std::map<std::string, std::string> row;
    if (!(inputStream >> row)) {
        row.clear();
    }
    return row;
}

std::vector<std::pair<std::string, std::string> > CSVInput::readEntryAsPairVector() {
    std::vector<std::pair<std::string, std::string> > values;
    if (!(inputStream >> values)) {
        values.clear();
    }
    return values;
}
