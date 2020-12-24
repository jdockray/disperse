
#ifndef DISPERSE_MATRICS
#define DISPERSE_MATRICS

#include <map>
#include "ExpectedException.hpp"

class SparseMatrix
{
public:
	SparseMatrix(unsigned int rows, unsigned int columns);
	double getValue(unsigned int row, unsigned int column) const;
	void setValue(unsigned int row, unsigned int column, double value);
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
	UpperTriangularSparseMatrix(unsigned int dimension);
	UpperTriangularSparseMatrix(const SparseMatrix& squareMatrix);
	void setValue(unsigned int row, unsigned int column, double value)
};

class DiagonalSparseMatrix : public UpperTriangularSparseMatrix
{
public:
	DiagonalSparseMatrix(unsigned int dimension);
	DiagonalSparseMatrix(const std::vector<double>& diagonalValues);
	void setValue(unsigned int diagonalPosition, double value);
	void setValue(unsigned int row, unsigned int column, double value);
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
	SparseMatrix product(a.columnCount(), b.rowCount());
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