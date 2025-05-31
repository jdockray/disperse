
#include "Elements.hpp"
#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <map>
#pragma warning(pop)

Element::Element(const std::string& row, const std::string& column)
	: row(row), column(column)
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

bool Element::operator==(const Element& other) const
{
	return row == other.row && column == other.column;
}

bool Element::operator!=(const Element& other) const
{
	return !operator==(other);
}

bool Element::operator<(const Element& other) const
{
	if (row < other.row)
	{
		return true;
	}
	else if (row == other.row)
	{
		return column < other.column;
	}
	else
	{
		return false;
	}
}

bool Element::operator<=(const Element& other) const
{
	return operator<(other) || operator==(other);
}

bool Element::operator>=(const Element& other) const
{
	return !operator<(other);
}

bool Element::operator>(const Element& other) const
{
	return !operator<=(other);
}

void ensureColumnInExpectedPlace(const std::vector<std::string> header, std::size_t index,
									const std::string& columnTitle,	const std::string& fileName)
{
	if (index >= header.size() || header.at(index) != columnTitle)
	{
		throw RequiredColumnNotFoundException(columnTitle, fileName);
	}
}

std::vector<std::pair<Element, double>> getElementsFromGridFile(IInput& input)
{
	std::vector<std::string> header = input.getHeader();
	ensureColumnInExpectedPlace(header, 0, "", input.getName());
	std::vector<std::pair<Element, double>> elements;
	std::string fileContext = "(" + input.getName() + ", ";
	std::vector<std::pair<std::string, std::string> > rowValues = input.readEntryAsPairVector();
	while (!rowValues.empty()) // Checks if vector is empty
	{
		std::vector<std::pair<std::string, std::string>>::const_iterator columnValue = rowValues.begin();
		std::string row = columnValue->second;
		std::string rowContext = fileContext + row + ", ";
		columnValue++;
		while (columnValue != rowValues.end())
		{
			elements.push_back(std::pair<Element, double>(
				Element(row, columnValue->first),
				CouldNotParseNumberException::convert(columnValue->second, rowContext + columnValue->first + ")")
			));
			columnValue++;
		}
		rowValues = input.readEntryAsPairVector();
	}
	return elements;
}

std::vector<std::pair<Element, double>> getElementsFromListFile(IInput& input)
{
	std::vector<std::string> header = input.getHeader();
	ensureColumnInExpectedPlace(header, 0, "Row", input.getName());
	ensureColumnInExpectedPlace(header, 1, "Column", input.getName());
	ensureColumnInExpectedPlace(header, 2, "Value", input.getName());
	std::vector<std::pair<std::string, std::string> > rowValues = input.readEntryAsPairVector();
	std::vector<std::pair<Element, double>> elements;
	while (!rowValues.empty()) // While vector is not empty
	{
		elements.push_back(std::pair<Element, double>(
			Element(rowValues.at(0).second, rowValues.at(1).second), std::stod(rowValues.at(2).second))
		);
		rowValues = input.readEntryAsPairVector();
	}
	return elements;
}

SparseMatrix elementMatrixFromVector(const std::vector<std::pair<Element, double>>& elements,
	std::vector<std::string>& placeForRowHeadings, std::vector<std::string>& placeForColumnHeadings)
{
	std::map<std::string, std::size_t> rowLookup;
	std::map<std::string, std::size_t> columnLookup;
	for (const std::pair<Element, double>& elementPair : elements)
	{
		std::map<std::string, std::size_t>::const_iterator rowHeading = rowLookup.find(elementPair.first.getRow());
		if (rowHeading == rowLookup.end())
		{
			rowLookup[elementPair.first.getRow()] = placeForRowHeadings.size();
			placeForRowHeadings.push_back(elementPair.first.getRow());
		}
		std::map<std::string, std::size_t>::const_iterator columnHeading = columnLookup.find(elementPair.first.getColumn());
		if (columnHeading == columnLookup.end())
		{
			columnLookup[elementPair.first.getColumn()] = placeForColumnHeadings.size();
			placeForColumnHeadings.push_back(elementPair.first.getColumn());
		}
	}
	SparseMatrix matrix(placeForRowHeadings.size(), placeForColumnHeadings.size());
	for (const std::pair<Element, double>& elementPair : elements)
	{
		matrix.setValue(rowLookup[elementPair.first.getRow()], columnLookup[elementPair.first.getColumn()], elementPair.second);
	}
	return matrix;
}
