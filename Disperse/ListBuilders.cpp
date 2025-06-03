
#include "Elements.hpp"
#include "ElementReaders.hpp"
#include "Input.hpp"
#include "ListBuilders.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include "dlib\dlib\dlib\matrix.h"
#pragma warning(pop)

void ensureColumnPresent(IInput& input, const std::string& columnName) {
    const std::vector headers = input.getHeader();
    if (std::find(headers.begin(), headers.end(), columnName) == headers.end()) {
        throw RequiredColumnNotFoundException(columnName, input.getName());
    }
}

ListOfSecurities SecurityListBuilder::inputSecurities(IInput& input) {
    ensureColumnPresent(input, security_column_name);
    ListOfSecurities securities;
    std::map<std::string, std::string> row = input.readEntryAsMap();
    while (!row.empty()) // Checks if map is empty
    {
        if (row.at(security_column_name).length() == 0) {
            continue;
        }
        Security security(row.at(security_column_name));
        std::map<std::string, std::string>::iterator expectedReturnColumn = row.find(expected_return_column_name);
        if (expectedReturnColumn != row.end()) {
            security.setExpectedReturn(std::stod(expectedReturnColumn->second));
        }
        std::map<std::string, std::string>::iterator riskColumn = row.find(risk_column_name);
        if (riskColumn != row.end()) {
            security.setRisk(std::stod(riskColumn->second));
        }
        std::map<std::string, std::string>::iterator minimumColumn = row.find(minimum_allocation_column_name);
        if (minimumColumn != row.end()) {
            security.setMinProportion(std::stod(minimumColumn->second));
        }
        std::map<std::string, std::string>::iterator maximumColumn = row.find(maximum_allocation_column_name);
        if (maximumColumn != row.end()) {
            security.setMaxProportion(std::stod(maximumColumn->second));
        }
        std::map<std::string, std::string>::iterator groupColumn = row.find(group_column_name);
        if (groupColumn != row.end()) {
            security.setGroup(groupColumn->second);
        }
        securities.add(security);
        row = input.readEntryAsMap();
    }
    securities.verifyProportions();
    return securities;
}

SecurityListBuilder::SecurityListBuilder(IInput& input)
    : securities(inputSecurities(input)) {
}

// In grid files: Row = Security, Column = Factor
// In list files: First column / Row = Security, SecondColumn / Column = Factor
void SecurityListBuilder::loadFactors(IReader& input) {
    for (const std::pair<Element, double>& element : input.getElements()) {
        securities.get(element.first.getRow()).addExposure(element.first.getColumn(), element.second);
    }
}

const ListOfSecurities& SecurityListBuilder::getSecurityList() {
    return securities;
}

ListOfGroups inputGroups(IInput& groupInput) {
    ListOfGroups groups;

    static const std::string group_column_name = "Group";
    static const std::string group_minimum_column_name = "Min";
    static const std::string group_maximum_column_name = "Max";

    ensureColumnPresent(groupInput, group_column_name);

    std::map<std::string, std::string> row = groupInput.readEntryAsMap();
    while (!row.empty()) // Tests if map is empty
    {
        const std::string groupIdentifier = row.at(group_column_name);
        if (groupIdentifier.length() == 0) continue;

        Group group(groupIdentifier);
        std::map<std::string, std::string>::iterator minimumColumn = row.find(group_minimum_column_name);
        if (minimumColumn != row.end()) {
            group.setMinProportion(std::stod(minimumColumn->second));
        }
        std::map<std::string, std::string>::iterator maximumColumn = row.find(group_maximum_column_name);
        if (maximumColumn != row.end()) {
            group.setMaxProportion(std::stod(maximumColumn->second));
        }

        groups.add(group);
        row = groupInput.readEntryAsMap();
    }
    return groups;
}
