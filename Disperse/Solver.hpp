
#ifndef DISPERSE_SOLVER
#define DISPERSE_SOLVER

#include "Matrices.hpp"

#pragma warning(push, 0)
#include "osqp\osqp\include\osqp.h"
#pragma warning(pop)

const unsigned int tolerance_decimal_places = 3;
const double rounding_multipler = std::pow(10.0, tolerance_decimal_places);

class ISolver {
public:
	virtual std::vector<double> solve(const SparseMatrix& matrixP, const SparseMatrix& matrixA,
		const std::vector<c_float>& vectorL, const std::vector<c_float>& vectorU) = 0;
};

class OSQPSolver : public ISolver {
public:
	OSQPSolver();
	virtual std::vector<double> solve(const SparseMatrix& matrixP, const SparseMatrix& matrixA,
		const std::vector<c_float>& vectorL, const std::vector<c_float>& vectorU);

private:
	OSQPSettings osqp_settings;
};

#endif // #ifndef DISPERSE_SOLVER
