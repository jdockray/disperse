
#include "Constraint.hpp"
#include "ConstraintUnpacking.hpp"
#include "Solver.hpp"

std::vector<double> solve(ISolver& solver, const SparseMatrix& covarianceMatrix, const std::vector<Constraint>& constraints)
{
	const std::size_t numberOfSecurities = covarianceMatrix.columnCount();
	std::vector<c_float> vectorL;
	std::vector<c_float> vectorU;
	SparseMatrix matrixA(constraints.size(), numberOfSecurities);
	for (const Constraint constraint : constraints)
	{
		for (std::size_t i = 0; i < numberOfSecurities; ++i)
		{
			matrixA.setValue(vectorL.size(), i, constraint.getWeight(i));
		}
		vectorL.push_back(constraint.getMinimum());
		vectorU.push_back(constraint.getMaximum());
	}

	// Optimisation minimises x'Px subject to l<=Ax<=u
	return solver.solve(/* Matrix P */ covarianceMatrix, matrixA, vectorL, vectorU);
}

double roundToOptimisationTolerance(double value)
{
	return std::round(value * rounding_multipler) / rounding_multipler;
}

std::vector<double> roundToOptimisationTolerance(const std::vector<double>& values)
{
	std::vector<double> roundedValues;
	roundedValues.reserve(values.size());
	for (double value : values)
	{
		roundedValues.push_back(roundToOptimisationTolerance(value));
	}
	return roundedValues;
}
