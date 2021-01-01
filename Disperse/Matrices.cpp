
#include "Matrices.hpp"

SparseMatrix::SparseMatrix(const unsigned int rows, const unsigned int columns)
	: numberOfRows(rows), numberOfColumns(columns)
{
}

double SparseMatrix::getValue(const unsigned int row, const unsigned int column) const
{
	auto matrixRow = elements.find(row);
	if (matrixRow == elements.end())
	{
		return 0;
	}
	auto cell = matrixRow->second.find(column);
	return (cell == matrixRow->second.end()) ? 0 : cell->second;
}

void SparseMatrix::setValue(const unsigned int row, const unsigned int column, const double value)
{
	if (column >= numberOfColumns || row >= numberOfRows)
	{
		throw UnexpectedException();
	}
	auto matrixRow = elements.find(row);
	if (matrixRow == elements.end())
	{
		elements[row] = std::map<unsigned int, double>();
		matrixRow = elements.find(row);
	}
	matrixRow->second[column] = value;
	if (matrixRow->second[column] == 0)
	{
		matrixRow->second.erase(column);
		if (matrixRow->second.size() == 0)
		{
			elements.erase(row);
		}
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

const std::map<unsigned int, std::map<unsigned int, double> >& SparseMatrix::matrixElements() const
{
	return elements;
}

SparseMatrix multiply(const SparseMatrix& a, const SparseMatrix& b)
{
	SparseMatrix product(a.columnCount(), b.rowCount());
	const SparseMatrix aTransposed = getTranspose(a);
	for (auto const& matrixARow : aTransposed.matrixElements())
	{
		for (auto const& matrixBColumn : b.matrixElements())
		{
			double sum = 0;
			for (auto const& matrixACell : matrixARow.second)
			{
				auto const& matrixBCell = matrixBColumn.second.find(matrixACell.first);
				if (matrixBCell == matrixBColumn.second.end()) continue;
				sum += matrixACell.second * matrixBCell->second;
			}
			product.setValue(matrixARow.first, matrixBColumn.first, sum);
		}
	}
	return product;
}

SparseMatrix getTranspose(const SparseMatrix& matrix)
{
	SparseMatrix transpose(matrix.columnCount(), matrix.rowCount());
	for (const auto& matrixRow : matrix.matrixElements())
	{
		for (const auto& cell : matrixRow.second)
		{
			transpose.setValue(cell.first, matrixRow.first, cell.second);
		}
	}
	return transpose;
}

SparseMatrix vectorToDiagonalMatrix(const std::vector<double>& values)
{
	SparseMatrix matrix(values.size(), values.size());
	for (int i = 0; i < values.size(); ++i)
	{
		matrix.setValue(i, i, values.at(i));
	}
	return matrix;
}

SparseMatrix vectorToHorizontalMatrix(const std::vector<double>& values)
{
	SparseMatrix matrix(1, values.size());
	for (int i = 0; i < values.size(); ++i)
	{
		matrix.setValue(0, i, values.at(i));
	}
	return matrix;
}

SparseMatrix vectorToVerticalMatrix(const std::vector<double>& values)
{
	SparseMatrix matrix(values.size(), 1);
	for (int i = 0; i < values.size(); ++i)
	{
		matrix.setValue(i, 0, values.at(i));
	}
	return matrix;
}

std::vector<double> horizontalMatrixToVector(const SparseMatrix& matrix)
{
	if (matrix.rowCount() != 1)
	{
		throw UnexpectedException();
	}
	std::vector<double> values(matrix.columnCount());
	for (int i = 0; i < matrix.columnCount(); ++i)
	{
		values.push_back(matrix.getValue(0, i));
	}
	return values;
}

std::vector<double> verticalMatrixToVector(const SparseMatrix& matrix)
{
	if (matrix.columnCount() != 1)
	{
		throw UnexpectedException();
	}
	std::vector<double> values(matrix.rowCount());
	for (int i = 0; i < matrix.rowCount(); ++i)
	{
		values.push_back(matrix.getValue(i, 0));
	}
	return values;
}


