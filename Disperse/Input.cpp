
#include "Input.hpp"
#include "../../csvstream/csvstream.h"
#include "Matrices.hpp"
#include "Security.hpp"
#include "dlib\matrix.h"

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
		securities.addSecurity(security);
	}
	securities.verifyProportions();
	return securities;
}

void inputFactorGrid(const std::string& inputFileName, ListOfSecurities& securities)
{
	csvstream inputStream(inputFileName);
	ensureColumnPresent(inputStream, security_column_name, inputFileName);
	std::map<std::string, std::string> row;
	while (inputStream >> row)
	{
		if (row.at(security_column_name).length() == 0) continue;
		Security& security = securities.getSecurity(row.at(security_column_name));
		for (std::string column : inputStream.getheader())
		{
			if (column.length() == 0) continue;
			security.addExposure(column, std::stod(row.at(column)));
		}
	}
}

void inputFactorList(const std::string& inputFileName, ListOfSecurities& securities)
{
	const std::string factor_column_name = "Factor";
	const std::string factor_weighting_column_name = "Weighting";

	csvstream inputStream(inputFileName);
	ensureColumnPresent(inputStream, security_column_name, inputFileName);
	ensureColumnPresent(inputStream, factor_column_name, inputFileName);
	ensureColumnPresent(inputStream, factor_weighting_column_name, inputFileName);
	std::map<std::string, std::string> row;
	while (inputStream >> row)
	{
		if (row.at(security_column_name).length() == 0) continue;
		securities.getSecurity(row.at(security_column_name)).addExposure(
			row.at(factor_column_name), std::stod(row.at(factor_weighting_column_name)));
	}
}

