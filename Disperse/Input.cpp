
#include "Input.hpp"
#include "Matrices.hpp"
#include "Elements.hpp"

#pragma warning(push, 0)
#include "csvstream\csvstream\csvstream.hpp"
#include "dlib\dlib\dlib\matrix.h"
#pragma warning(pop)

const std::string security_column_name = "Security";

void ensureColumnPresent(const csvstream& inputStream, const std::string& columnName,
	const std::string& fileName) // File name for reporting
{
	const std::vector headers = inputStream.getheader();
	if (std::find(headers.begin(), headers.end(), columnName) == headers.end())
	{
		throw RequiredColumnNotFoundException(columnName, fileName);
	}
}

ListOfSecurities inputSecurities(const std::string& inputFileName)
{
	const std::string expected_return_column_name = "Expected";
	const std::string risk_column_name = "Risk";
	const std::string minimum_allocation_column_name = "Min";
	const std::string maximum_allocation_column_name = "Max";
	const std::string group_column_name = "Group";

	csvstream inputStream(inputFileName);
	ensureColumnPresent(inputStream, security_column_name, inputFileName);
	ListOfSecurities securities;
	std::map<std::string, std::string> row;
	while (inputStream >> row)
	{
		if (row.at(security_column_name).length() == 0) continue;
		Security security(row.at(security_column_name));
		std::map<std::string, std::string>::iterator expectedReturnColumn = row.find(expected_return_column_name);
		if (expectedReturnColumn != row.end())
		{
			security.setExpectedReturn(std::stod(expectedReturnColumn->second));
		}
		std::map<std::string, std::string>::iterator riskColumn = row.find(risk_column_name);
		if (riskColumn != row.end())
		{
			security.setRisk(std::stod(riskColumn->second));
		}
		std::map<std::string, std::string>::iterator minimumColumn = row.find(minimum_allocation_column_name);
		if (minimumColumn != row.end())
		{
			security.setMinProportion(std::stod(minimumColumn->second));
		}
		std::map<std::string, std::string>::iterator maximumColumn = row.find(maximum_allocation_column_name);
		if (maximumColumn != row.end())
		{
			security.setMaxProportion(std::stod(maximumColumn->second));
		}
		std::map<std::string, std::string>::iterator groupColumn = row.find(group_column_name);
		if (groupColumn != row.end())
		{
			security.setGroup(groupColumn->second);
		}
		securities.add(security);
	}
	securities.verifyProportions();
	return securities;
}

// Row = Security, Column = Factor
void inputFactorGrid(const std::string& inputFileName, ListOfSecurities& securities)
{
	for (const std::pair<Element, double>& element : getElementsFromGridFile(inputFileName))
	{
		securities.get(element.first.getRow()).addExposure(element.first.getColumn(), element.second);
	}
}

// First column / Row = Security, SecondColumn / Column = Factor
void inputFactorList(const std::string& inputFileName, ListOfSecurities& securities)
{
	for (const std::pair<Element, double>& element : getElementsFromListFile(inputFileName))
	{
		securities.get(element.first.getRow()).addExposure(element.first.getColumn(), element.second);
	}
}

ListOfGroups inputGroups(const std::string& groupInputFileName)
{
	ListOfGroups groups;

	const std::string group_column_name = "Group";
	const std::string group_minimum_column_name = "Minimum";
	const std::string group_maximum_column_name = "Maximum";

	csvstream inputStream(groupInputFileName);
	ensureColumnPresent(inputStream, group_column_name, groupInputFileName);

	std::map<std::string, std::string> row;
	while (inputStream >> row)
	{
		const std::string groupIdentifier = row.at(group_column_name);
		if (groupIdentifier.length() == 0) continue;

		Group group(groupIdentifier);
		std::map<std::string, std::string>::iterator minimumColumn = row.find(group_minimum_column_name);
		if (minimumColumn != row.end())
		{
			group.setMinProportion(std::stod(minimumColumn->second));
		}
		std::map<std::string, std::string>::iterator maximumColumn = row.find(group_maximum_column_name);
		if (maximumColumn != row.end())
		{
			group.setMaxProportion(std::stod(maximumColumn->second));
		}
		
		groups.add(group);
	}
	return groups;
}
