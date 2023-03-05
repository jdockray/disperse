
#include "Elements.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include "../../csvstream/csvstream.h"
#include <map>
#pragma warning(pop)

Element::Element(const std::string& row, const std::string& column, double value)
	: row(row), column(column), value(value)
{
}

std::string Element::getColumn() const
{
	return column;
}

std::string Element::getRow() const
{
	return row;
}

double Element::getValue() const
{
	return value;
}

void ensureColumnInExpectedPlace(const std::vector<std::string> header, size_t index,
									const std::string& columnTitle,	const std::string& fileName)
{
	if (header.size() >= index || header.at(index) != columnTitle)
	{
		throw RequiredColumnNotFoundException(columnTitle, fileName);
	}
}

std::vector<Element> getElementsFromGridFile(const std::string& inputFileName)
{
	csvstream inputStream(inputFileName);
	std::vector<std::string> header = inputStream.getheader();
	ensureColumnInExpectedPlace(header, 0, "", inputFileName);
	std::vector<std::pair<std::string, std::string> > rowValues;
	std::vector<Element> elements;
	while (inputStream >> rowValues)
	{
		std::vector<std::pair<std::string, std::string>>::const_iterator columnValue = rowValues.begin();
		std::string row = (columnValue++)->second;
		while (columnValue != rowValues.end())
		{
			elements.push_back(Element(row, columnValue->first, std::stod(columnValue->second)));
		}
	}
	return elements;
}

std::vector<Element> getElementsFromListFile(const std::string& inputFileName)
{
	csvstream inputStream(inputFileName);
	std::vector<std::string> header = inputStream.getheader();
	ensureColumnInExpectedPlace(header, 0, "Row", inputFileName);
	ensureColumnInExpectedPlace(header, 1, "Column", inputFileName);
	ensureColumnInExpectedPlace(header, 2, "Value", inputFileName);
	std::vector<std::pair<std::string, std::string> > rowValues;
	std::vector<Element> elements;
	while (inputStream >> rowValues)
	{
		elements.push_back(Element(rowValues.at(0).second, rowValues.at(1).second,
											std::stod(rowValues.at(2).second)));
	}
	return elements;
}
