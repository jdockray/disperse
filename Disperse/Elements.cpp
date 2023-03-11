
#include "Elements.hpp"
#include "CSVOutput.hpp"
#include "ExpectedException.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include "../../csvstream/csvstream.h"
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

void ensureColumnInExpectedPlace(const std::vector<std::string> header, size_t index,
									const std::string& columnTitle,	const std::string& fileName)
{
	if (header.size() >= index || header.at(index) != columnTitle)
	{
		throw RequiredColumnNotFoundException(columnTitle, fileName);
	}
}

std::vector<std::pair<Element, double>> getElementsFromGridFile(const std::string& inputFileName)
{
	csvstream inputStream(inputFileName);
	std::vector<std::string> header = inputStream.getheader();
	ensureColumnInExpectedPlace(header, 0, "", inputFileName);
	std::vector<std::pair<std::string, std::string> > rowValues;
	std::vector<std::pair<Element, double>> elements;
	while (inputStream >> rowValues)
	{
		std::vector<std::pair<std::string, std::string>>::const_iterator columnValue = rowValues.begin();
		std::string row = (columnValue++)->second;
		while (columnValue != rowValues.end())
		{
			elements.push_back(std::pair<Element, double>(
				Element(row, columnValue->first), std::stod(columnValue->second))
			);
		}
	}
	return elements;
}

std::vector<std::pair<Element, double>> getElementsFromListFile(const std::string& inputFileName)
{
	csvstream inputStream(inputFileName);
	std::vector<std::string> header = inputStream.getheader();
	ensureColumnInExpectedPlace(header, 0, "Row", inputFileName);
	ensureColumnInExpectedPlace(header, 1, "Column", inputFileName);
	ensureColumnInExpectedPlace(header, 2, "Value", inputFileName);
	std::vector<std::pair<std::string, std::string> > rowValues;
	std::vector<std::pair<Element, double>> elements;
	while (inputStream >> rowValues)
	{
		elements.push_back(std::pair<Element, double>(
			Element(rowValues.at(0).second, rowValues.at(1).second), std::stod(rowValues.at(2).second))
		);
	}
	return elements;
}

void putElementsInListFile(const std::string& outputFileName, const std::vector<std::pair<Element, double>>& elements)
{
	CSVOutput csvOutput(outputFileName);
	csvOutput.writeElement("Row");
	csvOutput.writeElement("Column");
	csvOutput.writeElement("Value");
	csvOutput.finishLine();
	for (const std::pair<Element, double>& elementPair : elements)
	{
		if (elementPair.second != 0)
		{
			csvOutput.writeElement(elementPair.first.getRow());
			csvOutput.writeElement(elementPair.first.getColumn());
			csvOutput.writeElement(elementPair.second);
			csvOutput.finishLine();
		}
	}
}

void putElementsInGridFile(const std::string& outputFileName, const std::vector<std::string>& rowHeadings,
							const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix)
{
	if (rowHeadings.size() != matrix.rowCount() || columnHeadings.size() != matrix.columnCount())
	{
		throw UnexpectedException();
	}
	CSVOutput csvOutput(outputFileName);
	csvOutput.writeElement("");
	for (const std::string& column : columnHeadings)
	{
		csvOutput.writeElement(column);
	}
	csvOutput.finishLine();
	for (int rowIndex = 0; rowIndex < matrix.rowCount(); ++rowIndex)
	{
		csvOutput.writeElement(rowHeadings.at(rowIndex));
		for (int columnIndex = 0; columnIndex < matrix.columnCount(); ++columnIndex)
		{
			csvOutput.writeElement(matrix.getValue(rowIndex, columnIndex));
		}
		csvOutput.finishLine();
	}
}
