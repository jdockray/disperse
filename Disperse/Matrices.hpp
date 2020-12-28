
#ifndef DISPERSE_MATRICS
#define DISPERSE_MATRICS

#include <map>
#include "ExpectedException.hpp"

class SparseMatrix
{
public:
	SparseMatrix(const unsigned int rows, const unsigned int columns);
	double getValue(const unsigned int row, const unsigned int column) const;
	void setValue(const unsigned int row, const unsigned int column, const double value);
	unsigned int columnCount() const;
	unsigned int rowCount() const;
	const std::map<std::pair<unsigned int, unsigned int>, double>& matrixElements() const;
	SparseMatrix getTranspose() const;

private:
	const unsigned int numberOfColumns;
	const unsigned int numberOfRows;
	std::map<std::pair<unsigned int, unsigned int>, double> elements;
};

class UpperTriangularSparseMatrix : public SparseMatrix
{
public:
	UpperTriangularSparseMatrix(const unsigned int dimension);
	UpperTriangularSparseMatrix(const unsigned int rows, const unsigned int columns);
	UpperTriangularSparseMatrix(const SparseMatrix& squareMatrix);
	unsigned int sideLength() const;
	void setValue(const unsigned int row, const unsigned int column, const double value);
};

class UpperTriangularCorrelationMatrix : public UpperTriangularSparseMatrix
{
public:
	UpperTriangularCorrelationMatrix(const unsigned int dimension);
	UpperTriangularCorrelationMatrix(const unsigned int rows, const unsigned int columns);
	UpperTriangularCorrelationMatrix(const SparseMatrix& squareMatrix);
	void setValue(const unsigned int row, const unsigned int column, const double value);
private:
	void setDiagonal();
};

class DiagonalSparseMatrix : public UpperTriangularSparseMatrix
{
public:
	DiagonalSparseMatrix(const unsigned int dimension);
	DiagonalSparseMatrix(const unsigned int rows, const unsigned int columns);
	DiagonalSparseMatrix(const std::vector<double>& diagonalValues);
	void setValue(const unsigned int diagonalPosition, const double value);
	void setValue(const unsigned int row, const unsigned int column, const double value);
};

template<typename T>
T multiply(SparseMatrix a, SparseMatrix b)
{
	std::map<unsigned int, std::map<unsigned int, double> > matrixBLookup;
	for (auto const& elementOfB : b.matrixElements())
	{
		auto result = matrixBLookup.find(elementOfB.first.first);
		if (result == matrixBLookup.end())
		{
			matrixBLookup[elementOfB.first.first] = { {elementOfB.first.second, elementOfB.second} };
		}
		else
		{
			result->second.insert({ {elementOfB.first.second, elementOfB.second} });
		}
	}
	T product(a.columnCount(), b.rowCount());
	for (auto const& elementOfA : a.matrixElements())
	{
		auto const& matrixBRow = matrixBLookup.find(elementOfA.first.first);
		if (matrixBRow != matrixBLookup.end())
		{
			for (auto const& elementOfB : matrixBRow->second)
			{
				product.setValue(elementOfA.first.first, elementOfB.first, product.getValue(elementOfA.first.first, elementOfB.first) + elementOfA.second * elementOfB.second);
			}
		}
	}
	return product;
}

#endif // #ifndef DISPERSE_MATRICS