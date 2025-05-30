
#include "Elements.hpp"
#include "CSVOutput.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include "csvstream\csvstream\csvstream.hpp"
#include <map>
#pragma warning(pop)

const std::size_t MATRIX_SIZE_LIMIT = 25000000;

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

std::vector<std::pair<Element, double>> getElementsFromGridFile(const std::string& inputFileName)
{
	csvstream inputStream(inputFileName);
	std::vector<std::string> header = inputStream.getheader();
	ensureColumnInExpectedPlace(header, 0, "", inputFileName);
	std::vector<std::pair<std::string, std::string> > rowValues;
	std::vector<std::pair<Element, double>> elements;
	std::string fileContext = "(" + inputFileName + ", ";
	while (inputStream >> rowValues)
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

void putElementsInListFile(AbstractCSVOutput& csvOutput, const std::vector<std::pair<Element, double>>& elements)
{
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
	for (const std::pair<std::size_t, std::map<std::size_t, double>>& row : matrix.matrixElements())
	{
		for (const std::pair<std::size_t, double>& element : row.second)
		{
			elements.push_back(std::pair<Element, double>(
				Element(rowHeadings.at(row.first), columnHeadings.at(element.first)),
				element.second
			));
		}
	}
	return elements;
}

void putElementsInListFile(AbstractCSVOutput& csvOutput, const std::vector<std::string>& rowHeadings,
	const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix)
{
	putElementsInListFile(csvOutput, elementVectorFromMatrix(rowHeadings, columnHeadings, matrix));
}

void putElementsInGridFile(AbstractCSVOutput& csvOutput, const std::vector<std::string>& rowHeadings,
							const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix)
{
	const std::size_t numberOfRows = matrix.rowCount();
	const std::size_t numberOfColumns = matrix.columnCount();
	if (rowHeadings.size() != numberOfRows || columnHeadings.size() != numberOfColumns)
	{
		throw UnexpectedException();
	}
	if (numberOfRows * numberOfColumns > MATRIX_SIZE_LIMIT)
	{
		throw ExcessiveSizeException("Matrix of " + std::to_string(numberOfRows) + " x "
										+ std::to_string(numberOfColumns) + " exceeds size limit.");
	}
	csvOutput.writeElement("");
	for (const std::string& column : columnHeadings)
	{
		csvOutput.writeElement(column);
	}
	csvOutput.finishLine();
	for (unsigned int rowIndex = 0; rowIndex < matrix.rowCount(); ++rowIndex)
	{
		csvOutput.writeElement(rowHeadings.at(rowIndex));
		for (unsigned int columnIndex = 0; columnIndex < matrix.columnCount(); ++columnIndex)
		{
			csvOutput.writeElement(matrix.getValue(rowIndex, columnIndex));
		}
		csvOutput.finishLine();
	}
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

void putElementsInGridFile(AbstractCSVOutput& csvOutput, const std::vector<std::pair<Element, double>>& elements)
{
	std::vector<std::string> rowHeadings;
	std::vector<std::string> columnHeadings;
	putElementsInGridFile(csvOutput, rowHeadings, columnHeadings,
		elementMatrixFromVector(elements, rowHeadings, columnHeadings));
}
