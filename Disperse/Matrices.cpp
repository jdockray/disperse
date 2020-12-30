
#include "Matrices.hpp"

SparseMatrix::SparseMatrix(const unsigned int rows, const unsigned int columns)
	: numberOfRows(rows), numberOfColumns(columns)
{
}

double SparseMatrix::getValue(const unsigned int row, const unsigned int column) const
{
	auto result = elements.find(std::pair<unsigned int, unsigned int>(row, column));
	return result == elements.end() ? 0 : result->second;
}

void SparseMatrix::setValue(const unsigned int row, const unsigned int column, const double value)
{
	if (column >= numberOfColumns || row >= numberOfRows)
	{
		throw UnexpectedException();
	}
	const std::pair<unsigned int, unsigned int> key(row, column);
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

DiagonalSparseMatrix::DiagonalSparseMatrix(const unsigned int dimension)
	: UpperTriangularSparseMatrix(dimension)
{
}

DiagonalSparseMatrix::DiagonalSparseMatrix(const unsigned int rows, const unsigned int columns)
	: UpperTriangularSparseMatrix(rows, columns)
{
}

DiagonalSparseMatrix::DiagonalSparseMatrix(const std::vector<double>& diagonalValues)
	: DiagonalSparseMatrix(static_cast<unsigned int>(diagonalValues.size()))
{
	for (unsigned int i = 0; i < diagonalValues.size(); ++i)
	{
		UpperTriangularSparseMatrix::setValue(i, i, diagonalValues.at(i));
	}
}

void DiagonalSparseMatrix::setValue(const unsigned int diagonalPosition, const double value)
{
	UpperTriangularSparseMatrix::setValue(diagonalPosition, diagonalPosition, value);
}

void DiagonalSparseMatrix::setValue(const unsigned int row, const unsigned int column, const double value)
{
	if (row != column)
	{
		throw UnexpectedException();
	}
	setValue(row, value);
}

UpperTriangularSparseMatrix::UpperTriangularSparseMatrix(const unsigned int dimension)
	: SparseMatrix(dimension, dimension)
{
}

UpperTriangularSparseMatrix::UpperTriangularSparseMatrix(const unsigned int rows, const unsigned int columns)
	: SparseMatrix(rows, columns)
{
	if (rows != columns)
	{
		throw UnexpectedException();
	}
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

unsigned int UpperTriangularSparseMatrix::sideLength() const
{
	return rowCount();
}

void UpperTriangularSparseMatrix::setValue(const unsigned int row, const unsigned int column, const double value)
{
	unsigned int storageRow;
	unsigned int storageColumn;
	if (row > column)
	{
		// Incorrect. Swap over.
		storageRow = column;
		storageColumn = row;
	}
	else
	{
		storageRow = row;
		storageColumn = column;
	}
	SparseMatrix::setValue(row, column, value);
}

void UpperTriangularCorrelationMatrix::setValue(const unsigned int row, const unsigned int column, const double value)
{
	if (row != column)
	{
		UpperTriangularCorrelationMatrix::setValue(row, column, value);
	}
}

UpperTriangularCorrelationMatrix::UpperTriangularCorrelationMatrix(const unsigned int dimension)
	: UpperTriangularSparseMatrix(dimension, dimension)
{
	setDiagonal();
}

UpperTriangularCorrelationMatrix::UpperTriangularCorrelationMatrix(const unsigned int rows, const unsigned int columns)
	: UpperTriangularSparseMatrix(rows, columns)
{
	if (rows != columns)
	{
		throw UnexpectedException();
	}
	setDiagonal();
}

UpperTriangularCorrelationMatrix::UpperTriangularCorrelationMatrix(const SparseMatrix& squareMatrix)
	: UpperTriangularSparseMatrix(squareMatrix.rowCount(), squareMatrix.rowCount())
{
	if (rowCount() != columnCount())
	{
		throw UnexpectedException();
	}
	setDiagonal();
	for (const auto& element : squareMatrix.matrixElements())
	{
		if (element.first.first > element.first.second)
		{
			setValue(element.first.second, element.first.first, element.second);
		}
	}
}

void UpperTriangularCorrelationMatrix::setDiagonal()
{
	for (unsigned int i = 0; i < sideLength(); ++i)
	{
		UpperTriangularSparseMatrix::setValue(i, i, 1);
	}
}

SparseVector::SparseVector(const unsigned int length)
	: SparseMatrix(length, 1)
{
}

SparseVector::SparseVector(const unsigned int rows, const unsigned int columns)
	: SparseMatrix(rows, 1)
{
	if (columns != 1)
	{
		throw UnexpectedException();
	}
}

SparseVector::SparseVector(const std::vector<double>& values)
	: SparseMatrix(static_cast<unsigned int>(values.size()), 1)
{
	for (unsigned int i = 0; i < values.size(); ++i)
	{
		setValue(i, values.at(i));
	}
}

void SparseVector::setValue(const unsigned int index, const double value)
{
	SparseMatrix::setValue(index, 0, value);
}

void SparseVector::setValue(const unsigned int row, const unsigned int column, const double value)
{
	if (column != 0)
	{
		throw UnexpectedException();
	}
	setValue(row, value);
}

std::vector<double> SparseVector::asVector() const
{
	std::vector<double> values;
	for (unsigned int i = 0; i < values.size(); ++i)
	{
		values.push_back(getValue(i, 1));
	}
	return values;
}
