
#include "Matrices.hpp"

#include "gtest/gtest.h"

TEST(SparseMatrix, Empty) {
	SparseMatrix matrix(0, 0);
	EXPECT_EQ(matrix.columnCount(), 0);
	EXPECT_EQ(matrix.rowCount(), 0);
}
