
#include "Output.hpp"
#include "Elements.hpp"
#include "ElementWriters.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

const std::size_t MATRIX_SIZE_LIMIT = 25000000;

ListFileWriter::ListFileWriter(IOutput& output)
	: output(output)
{
}

void ListFileWriter::writeElements(const std::vector<std::pair<Element, double>>& elements) {
	output.writeElement("Row");
	output.writeElement("Column");
	output.writeElement("Value");
	output.finishLine();
	for (const std::pair<Element, double>& elementPair : elements)
	{
		if (elementPair.second != 0)
		{
			output.writeElement(elementPair.first.getRow());
			output.writeElement(elementPair.first.getColumn());
			output.writeElement(elementPair.second);
			output.finishLine();
		}
	}
}

void ListFileWriter::writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix) {
	writeElements(elementVectorFromMatrix(rowHeadings, columnHeadings, matrix));
}

std::vector<std::pair<Element, double>> ListFileWriter::elementVectorFromMatrix(const std::vector<std::string>& rowHeadings,
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

GridFileWriter::GridFileWriter(IOutput& output)
	: output(output)
{
}

void GridFileWriter::writeElements(const std::vector<std::pair<Element, double>>& elements) {
	std::vector<std::string> rowHeadings;
	std::vector<std::string> columnHeadings;
	writeElements(rowHeadings, columnHeadings, elementMatrixFromVector(elements, rowHeadings, columnHeadings));
}

void GridFileWriter::writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix) {
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
	output.writeElement("");
	for (const std::string& column : columnHeadings)
	{
		output.writeElement(column);
	}
	output.finishLine();
	for (unsigned int rowIndex = 0; rowIndex < matrix.rowCount(); ++rowIndex)
	{
		output.writeElement(rowHeadings.at(rowIndex));
		for (unsigned int columnIndex = 0; columnIndex < matrix.columnCount(); ++columnIndex)
		{
			output.writeElement(matrix.getValue(rowIndex, columnIndex));
		}
		output.finishLine();
	}
}

