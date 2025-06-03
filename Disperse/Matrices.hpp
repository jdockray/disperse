
#ifndef DISPERSE_MATRICS
#define DISPERSE_MATRICS

#include "Exceptions.hpp"

#pragma warning(push, 0)
#include <map>
#include <functional>
#pragma warning(pop)

class SparseMatrix {
public:
    SparseMatrix(const std::size_t rows, const std::size_t columns);
    double getValue(const std::size_t row, const std::size_t column) const;
    void setValue(const std::size_t row, const std::size_t column, const double value);
    std::size_t columnCount() const;
    std::size_t rowCount() const;
    const std::map<std::size_t, std::map<std::size_t, double> >& matrixElements() const;

private:
    SparseMatrix(const std::size_t rows, const std::size_t columns,
        const std::map<std::size_t, std::map<std::size_t, double> >& elements);

    std::size_t numberOfColumns;
    std::size_t numberOfRows;
    std::map<std::size_t, std::map<std::size_t, double> > elements;
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
