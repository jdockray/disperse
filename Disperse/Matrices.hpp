
#ifndef DISPERSE_MATRICS
#define DISPERSE_MATRICS

#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <map>
#include <functional>
#pragma warning(pop)

class SparseMatrix
{
public:
	SparseMatrix(const size_t rows, const size_t columns);
	double getValue(const size_t row, const size_t column) const;
	void setValue(const size_t row, const size_t column, const double value);
	size_t columnCount() const;
	size_t rowCount() const;
	const std::map<size_t, std::map<size_t, double> >& matrixElements() const;

private:
	const size_t numberOfColumns;
	const size_t numberOfRows;
	std::map<size_t, std::map<size_t, double> > elements;
};

SparseMatrix multiply(const SparseMatrix& a, const SparseMatrix& b);
SparseMatrix multiply(const SparseMatrix& a, double scalar);
SparseMatrix multiply(const SparseMatrix& a, const SparseMatrix& b, const SparseMatrix& c);
SparseMatrix getTranspose(const SparseMatrix& matrix);
void applyToAllNonZeroElements(SparseMatrix& matrix, std::function<double(double)> function);
SparseMatrix upperTriangularMatrix(const SparseMatrix& matrix);
SparseMatrix vectorToDiagonalMatrix(const std::vector<double>& values);
SparseMatrix vectorToHorizontalMatrix(const std::vector<double>& values);
SparseMatrix vectorToVerticalMatrix(const std::vector<double>& values);
std::vector<double> horizontalMatrixToVector(const SparseMatrix& matrix);
std::vector<double> verticalMatrixToVector(const SparseMatrix& matrix);
double getSingleValue(const SparseMatrix& matrix);

#endif // #ifndef DISPERSE_MATRICS
