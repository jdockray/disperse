
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
	const std::map<unsigned int, std::map<unsigned int, double> >& matrixElements() const;

private:
	const unsigned int numberOfColumns;
	const unsigned int numberOfRows;
	std::map<unsigned int, std::map<unsigned int, double> > elements;
};

SparseMatrix multiply(const SparseMatrix& a, const SparseMatrix& b);
SparseMatrix getTranspose(const SparseMatrix& matrix);
SparseMatrix upperTriangularMatrix(const SparseMatrix& matrix);
SparseMatrix vectorToDiagonalMatrix(const std::vector<double>& values);
SparseMatrix vectorToHorizontalMatrix(const std::vector<double>& values);
SparseMatrix vectorToVerticalMatrix(const std::vector<double>& values);
std::vector<double> horizontalMatrixToVector(const SparseMatrix& matrix);
std::vector<double> verticalMatrixToVector(const SparseMatrix& matrix);

#endif // #ifndef DISPERSE_MATRICS
