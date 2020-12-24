
#include "Matrices.hpp"

SparseMatrix::SparseMatrix(unsigned int rows, unsigned int columns)
	: numberOfRows(rows), numberOfColumns(columns)
{
}

double SparseMatrix::getValue(unsigned int row, unsigned int column) const
{
	auto result = elements.find(std::pair<unsigned int, unsigned int>(column, row));
	return result == elements.end() ? 0 : result->second;
}

void SparseMatrix::setValue(unsigned int row, unsigned int column, double value)
{
	if (column >= numberOfColumns || row >= numberOfRows)
	{
		throw UnexpectedException();
	}
	const std::pair<unsigned int, unsigned int> key(column, row);
	if (value == 0)
	{
		elements.erase(key);
	}
	else
	{
		elements[key] = value;
	}
}

unsigned int SparseMatrix::columnCount() const
{
	return numberOfColumns;
}

unsigned int SparseMatrix::rowCount() const
{
	return numberOfRows;
}

const std::map<std::pair<unsigned int, unsigned int>, double>& SparseMatrix::matrixElements() const
{
	return elements;
}

SparseMatrix SparseMatrix::getTranspose() const
{
	SparseMatrix transpose(columnCount(), rowCount());
	for (const auto& element : matrixElements())
	{
		transpose.setValue(element.first.second, element.first.first, element.second);
	}
	return transpose;
}

DiagonalSparseMatrix::DiagonalSparseMatrix(unsigned int dimension)
	: UpperTriangularSparseMatrix(dimension)
{
}

DiagonalSparseMatrix::DiagonalSparseMatrix(const std::vector<double>& diagonalValues)
	: DiagonalSparseMatrix(diagonalValues.size())
{
	for (unsigned int i = 0; i < diagonalValues.size(); ++i)
	{
		UpperTriangularSparseMatrix::setValue(i, i, diagonalValues.at(i));
	}
}

void DiagonalSparseMatrix::setValue(unsigned int diagonalPosition, double value)
{
	UpperTriangularSparseMatrix::setValue(diagonalPosition, diagonalPosition, value);
}

void DiagonalSparseMatrix::setValue(unsigned int row, unsigned int column, double value)
{
	if (row != column)
	{
		throw UnexpectedException();
	}
	setValue(row, value);
}

UpperTriangularSparseMatrix::UpperTriangularSparseMatrix(unsigned int dimension)
	: SparseMatrix(dimension, dimension)
{
}

UpperTriangularSparseMatrix::UpperTriangularSparseMatrix(const SparseMatrix& squareMatrix)
	: SparseMatrix(squareMatrix.rowCount(), squareMatrix.rowCount())
{
	if (rowCount() != columnCount())
	{
		throw UnexpectedException();
	}
	for (const auto& element : squareMatrix.matrixElements())
	{
		if (element.first.first >= element.first.second)
		{
			setValue(element.first.second, element.first.first, element.second);
		}
	}
}

void UpperTriangularSparseMatrix::setValue(unsigned int row, unsigned int column, double value)
{
	if (row > column)
	{
		throw UnexpectedException();
	}
	SparseMatrix::setValue(row, column, value);
}
