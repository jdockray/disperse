
#include "Elements.hpp"
#include "CSVOutput.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include "../../csvstream/csvstream.h"
#include <map>
#pragma warning(pop)

const size_t MATRIX_SIZE_LIMIT = 25000000;

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

std::vector<std::pair<Element, double>> elementVectorFromMatrix(const std::vector<std::string>& rowHeadings,
	const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix)
{
	std::vector<std::pair<Element, double>> elements;
	for (const std::pair<size_t, std::map<size_t, double>>& row : matrix.matrixElements())
	{
		for (const std::pair<size_t, double>& element : row.second)
		{
			elements.push_back(std::pair<Element, double>(
				Element(rowHeadings.at(row.first), columnHeadings.at(element.first)),
				element.second
			));
		}
	}
	return elements;
}

void putElementsInListFile(const std::string& outputFileName, const std::vector<std::string>& rowHeadings,
	const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix)
{
	putElementsInListFile(outputFileName, elementVectorFromMatrix(rowHeadings, columnHeadings, matrix));
}

void putElementsInGridFile(const std::string& outputFileName, const std::vector<std::string>& rowHeadings,
							const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix)
{
	const size_t numberOfRows = matrix.rowCount();
	const size_t numberOfColumns = matrix.columnCount();
	if (rowHeadings.size() != numberOfRows || columnHeadings.size() != numberOfColumns)
	{
		throw UnexpectedException();
	}
	if (numberOfRows * numberOfColumns > MATRIX_SIZE_LIMIT)
	{
		throw ExcessiveSizeException("Matrix of " + std::to_string(numberOfRows) + " x "
										+ std::to_string(numberOfColumns) + " exceeds size limit.");
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

SparseMatrix elementMatrixFromVector(const std::vector<std::pair<Element, double>>& elements,
	std::vector<std::string>& placeForRowHeadings, std::vector<std::string>& placeForColumnHeadings)
{
	std::map<std::string, size_t> rowLookup;
	std::map<std::string, size_t> columnLookup;
	for (const std::pair<Element, double>& elementPair : elements)
	{
		std::map<std::string, size_t>::const_iterator rowHeading = rowLookup.find(elementPair.first.getRow());
		if (rowHeading == rowLookup.end())
		{
			rowLookup[elementPair.first.getRow()] = placeForRowHeadings.size();
			placeForRowHeadings.push_back(elementPair.first.getRow());
		}
		std::map<std::string, size_t>::const_iterator columnHeading = columnLookup.find(elementPair.first.getColumn());
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

void putElementsInGridFile(const std::string& outputFileName, const std::vector<std::pair<Element, double>>& elements)
{
	std::vector<std::string> rowHeadings;
	std::vector<std::string> columnHeadings;
	putElementsInGridFile(outputFileName, rowHeadings, columnHeadings,
		elementMatrixFromVector(elements, rowHeadings, columnHeadings));
}
