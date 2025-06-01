
#include "Matrices.hpp"

#include "gtest/gtest.h"

TEST(SparseMatrix, ZeroSize) {
	SparseMatrix matrix(0, 0);
	EXPECT_EQ(matrix.columnCount(), 0);
	EXPECT_EQ(matrix.rowCount(), 0);
	EXPECT_EQ(matrix.matrixElements().size(), 0);
}

TEST(SparseMatrix, NoZeroHeightOrWidthMatrix) {
	EXPECT_EQ(SparseMatrix(0, 1).columnCount(), 0);
	EXPECT_EQ(SparseMatrix(1, 0).rowCount(), 0);
}

TEST(SparseMatrix, ZeroSizeOutOfRangeFail) {
	SparseMatrix matrix(0, 0);
	EXPECT_THROW(matrix.getValue(0, 0), std::exception);
}

TEST(SparseMatrix, MaxSizeNoElements) {
	SparseMatrix matrix(std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max());
	EXPECT_EQ(matrix.getValue(0, 0), 0);
	EXPECT_EQ(matrix.getValue(2, 4), 0);
	EXPECT_EQ(matrix.getValue(std::numeric_limits<std::size_t>::max() - 1, std::numeric_limits<std::size_t>::max() - 1), 0);
	EXPECT_EQ(matrix.matrixElements().size(), 0);
	EXPECT_EQ(matrix.rowCount(), std::numeric_limits<std::size_t>::max());
	EXPECT_EQ(matrix.columnCount(), std::numeric_limits<std::size_t>::max());
}

TEST(SparseMatrix, SetAndGet) {
	SparseMatrix matrix(3, 5);
	matrix.setValue(2, 0, std::numeric_limits<double>::infinity());
	matrix.setValue(0, 4, std::numeric_limits<double>::epsilon());
	EXPECT_EQ(matrix.getValue(0, 4), std::numeric_limits<double>::epsilon());
	matrix.setValue(1, 4, std::numeric_limits<double>::min());
	EXPECT_EQ(matrix.getValue(2, 0), std::numeric_limits<double>::infinity());
	EXPECT_EQ(matrix.getValue(1, 4), std::numeric_limits<double>::min());
}

TEST(SparseMatrix, SetThenClear) {
	SparseMatrix matrix(1, 1);
	EXPECT_EQ(matrix.getValue(0, 0), 0);
	matrix.setValue(0, 0, 1);
	EXPECT_EQ(matrix.getValue(0, 0), 1);
	matrix.setValue(0, 0, 0);
	EXPECT_EQ(matrix.matrixElements().size(), 0);
	EXPECT_EQ(matrix.getValue(0, 0), 0);
}

TEST(SparseMatrix, Map) {
	SparseMatrix matrix(7, 17);
	matrix.setValue(1, 16, std::numeric_limits<double>::epsilon());
	EXPECT_EQ(matrix.matrixElements().size(), 1);
	EXPECT_EQ(matrix.matrixElements().at(1).size(), 1);
	EXPECT_EQ(matrix.matrixElements().at(1).at(16), std::numeric_limits<double>::epsilon());
}

TEST(SparseMatrix, SetThenClearInMap) {
	SparseMatrix matrix(2, 2);
	matrix.setValue(0, 0, 1);
	matrix.setValue(0, 1, 1);
	matrix.setValue(1, 0, 1);
	matrix.setValue(1, 1, 1);	
	EXPECT_EQ(matrix.getValue(0, 0), 1);
	matrix.setValue(0, 0, 0);
	ASSERT_EQ(matrix.matrixElements().size(), 2);
	EXPECT_EQ(matrix.matrixElements().at(0).size(), 1);
}

TEST(SparseMatrix, SetThenClearRowInMap) {
	SparseMatrix matrix(2, 2);
	matrix.setValue(0, 0, 1);
	matrix.setValue(0, 1, 1);
	matrix.setValue(1, 0, 1);
	matrix.setValue(1, 1, 1);
	matrix.setValue(0, 0, 0);
	matrix.setValue(0, 0, 0);
	matrix.setValue(0, 1, 0);
	EXPECT_EQ(matrix.matrixElements().size(), 1);
}

/*
SparseMatrix multiply(const SparseMatrix& a, const SparseMatrix& b, const SparseMatrix& c);
SparseMatrix upperTriangularMatrix(const SparseMatrix& matrix);
*/

TEST(MultiplyMatrices, IncompatibleDimensionsFail) {
	EXPECT_THROW(multiply(SparseMatrix(1, 2), SparseMatrix(3, 1)), std::exception);
}

TEST(MultiplyMatrices, Success) {
	SparseMatrix matrixA(2, 1);
	matrixA.setValue(0, 0, 5);
	matrixA.setValue(1, 0, 7);
	SparseMatrix matrixB(1, 2);
	matrixB.setValue(0, 0, 2);
	matrixB.setValue(0, 1, 3);
	SparseMatrix multiplied = multiply(matrixA, matrixB);
	EXPECT_EQ(multiplied.getValue(0, 0), 10);
	EXPECT_EQ(multiplied.getValue(0, 1), 15);
	EXPECT_EQ(multiplied.getValue(1, 0), 14);
	EXPECT_EQ(multiplied.getValue(1, 1), 21);
	EXPECT_EQ(getSingleValue(multiply(matrixB, matrixA)), 31);
}

TEST(MultiplyByScalar, Success) {
	SparseMatrix matrix(2, 2);
	matrix.setValue(0, 0, -1);
	matrix.setValue(0, 1, -1);
	matrix.setValue(1, 0, -1);
	matrix.setValue(1, 1, -1);
	SparseMatrix multiplied = multiply(matrix, -1);
	EXPECT_EQ(multiplied.getValue(0, 0), 1);
	EXPECT_EQ(multiplied.getValue(0, 1), 1);
	EXPECT_EQ(multiplied.getValue(1, 0), 1);
	EXPECT_EQ(multiplied.getValue(1, 1), 1);
}

TEST(GetTranspose, Success) {
	SparseMatrix matrix(2, 3);
	matrix.setValue(0, 0, 9);
	matrix.setValue(0, 1, 0);
	matrix.setValue(0, 2, 1);
	matrix.setValue(1, 0, 2);
	matrix.setValue(1, 1, 3);
	matrix.setValue(1, 2, 4);
	SparseMatrix transpose = getTranspose(matrix);
	EXPECT_EQ(transpose.columnCount(), matrix.rowCount());
	EXPECT_EQ(transpose.rowCount(), matrix.columnCount());
	EXPECT_EQ(transpose.getValue(0, 0), matrix.getValue(0, 0));
	EXPECT_EQ(transpose.getValue(0, 1), matrix.getValue(1, 0));
	EXPECT_EQ(transpose.getValue(1, 0), matrix.getValue(0, 1));
	EXPECT_EQ(transpose.getValue(1, 1), matrix.getValue(1, 1));
	EXPECT_EQ(transpose.getValue(2, 0), matrix.getValue(0, 2));
	EXPECT_EQ(transpose.getValue(2, 1), matrix.getValue(1, 2));
}

TEST(ApplyToAllNonZeroElements, Success) {
	SparseMatrix matrix(2, 2);
	matrix.setValue(0, 1, 1);
	matrix.setValue(1, 0, 1);
	matrix.setValue(1, 1, 1);
	applyToAllNonZeroElements(matrix, [](double x){ return x + 1; });
	EXPECT_EQ(matrix.getValue(0, 0), 0);
	EXPECT_EQ(matrix.getValue(0, 1), 2);
	EXPECT_EQ(matrix.getValue(1, 0), 2);
	EXPECT_EQ(matrix.getValue(1, 1), 2);
}

TEST(VectorToDiagonalMatrix, ZeroSize) {
	SparseMatrix matrix = vectorToDiagonalMatrix(std::vector<double>());
	EXPECT_EQ(matrix.columnCount(), 0);
	EXPECT_EQ(matrix.rowCount(), 0);
}

TEST(VectorToDiagonalMatrix, Success) {
	SparseMatrix matrix = vectorToDiagonalMatrix(std::vector<double>(2, 1));
	EXPECT_EQ(matrix.columnCount(), 2);
	EXPECT_EQ(matrix.rowCount(), 2);
	EXPECT_EQ(matrix.getValue(0, 0), 1);
	EXPECT_EQ(matrix.getValue(0, 1), 0);
	EXPECT_EQ(matrix.getValue(1, 0), 0);
	EXPECT_EQ(matrix.getValue(1, 1), 1);
}

TEST(VectorToHorizontalMatrix, ZeroSize) {
	SparseMatrix matrix = vectorToHorizontalMatrix(std::vector<double>());
	EXPECT_EQ(matrix.columnCount(), 0);
	EXPECT_EQ(matrix.rowCount(), 0);
}

TEST(VectorToHorizontalMatrix, Success) {
	SparseMatrix matrix = vectorToHorizontalMatrix(std::vector<double>(2, 1));
	EXPECT_EQ(matrix.columnCount(), 2);
	EXPECT_EQ(matrix.rowCount(), 1);
	EXPECT_EQ(matrix.getValue(0, 0), 1);
	EXPECT_EQ(matrix.getValue(0, 1), 1);
}

TEST(VectorToVerticalMatrix, ZeroSize) {
	SparseMatrix matrix = vectorToVerticalMatrix(std::vector<double>());
	EXPECT_EQ(matrix.columnCount(), 0);
	EXPECT_EQ(matrix.rowCount(), 0);
}

TEST(VectorToVerticalMatrix, Success) {
	SparseMatrix matrix = vectorToVerticalMatrix(std::vector<double>(2, 1));
	EXPECT_EQ(matrix.columnCount(), 1);
	EXPECT_EQ(matrix.rowCount(), 2);
	EXPECT_EQ(matrix.getValue(0, 0), 1);
	EXPECT_EQ(matrix.getValue(1, 0), 1);
}

TEST(HorizontalMatrixToVector, ZeroSizeFail) {
	EXPECT_THROW(horizontalMatrixToVector(SparseMatrix(0, 0)), std::exception);
}

TEST(HorizontalMatrixToVector, MoreThanOneRowFail) {
	EXPECT_THROW(horizontalMatrixToVector(SparseMatrix(2, 1)), std::exception);
}

TEST(HorizontalMatrixToVector, NoValues) {
	EXPECT_EQ(horizontalMatrixToVector(SparseMatrix(1, 5)).size(), 5);
}

TEST(HorizontalMatrixToVector, Success) {
	SparseMatrix matrix(1, 2);
	matrix.setValue(0, 0, 12);
	std::vector<double> vector = horizontalMatrixToVector(matrix);
	ASSERT_EQ(vector.size(), 2);
	EXPECT_EQ(vector.at(0), 12);
	EXPECT_EQ(vector.at(1), 0);
}

TEST(VerticalMatrixToVector, ZeroSizeFail) {
	EXPECT_THROW(verticalMatrixToVector(SparseMatrix(0, 0)), std::exception);
}

TEST(VerticalMatrixToVector, MoreThanOneColumnFail) {
	EXPECT_THROW(verticalMatrixToVector(SparseMatrix(1, 2)), std::exception);
}

TEST(VerticalMatrixToVector, NoValues) {
	EXPECT_EQ(verticalMatrixToVector(SparseMatrix(3, 1)).size(), 3);
}

TEST(VerticalMatrixToVector, Success) {
	SparseMatrix matrix(2, 1);
	matrix.setValue(0, 0, 12);
	std::vector<double> vector = verticalMatrixToVector(matrix);
	ASSERT_EQ(vector.size(), 2);
	EXPECT_EQ(vector.at(0), 12);
	EXPECT_EQ(vector.at(1), 0);
}

TEST(MatrixSingleValue, WithSetValue) {
	SparseMatrix matrix(1, 1);
	matrix.setValue(0, 0, 34);
	EXPECT_EQ(getSingleValue(matrix), 34);
}

TEST(MatrixSingleValue, WithNoSetValue) {
	EXPECT_EQ(getSingleValue(SparseMatrix(1, 1)), 0);
}

TEST(MatrixSingleValue, SizeFailure) {
	EXPECT_THROW(getSingleValue(SparseMatrix(0, 0)), std::exception);
	EXPECT_THROW(getSingleValue(SparseMatrix(1, 3)), std::exception);
	EXPECT_THROW(getSingleValue(SparseMatrix(0, 1)), std::exception);
}
