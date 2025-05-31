
#ifndef DISPERSE_CONSTRAINT_UNPACKING
#define DISPERSE_CONSTRAINT_UNPACKING

#include "Constraint.hpp"
#include "Group.hpp"
#include "Matrices.hpp"
#include "Security.hpp"
#include "Solver.hpp"

#pragma warning(push, 0)
#include "dlib\dlib\dlib\matrix.h"
#include <vector>
#pragma warning(pop)

double roundToOptimisationTolerance(double value);
std::vector<double> roundToOptimisationTolerance(const std::vector<double>& values);

std::vector<double> solve(ISolver& solver, const SparseMatrix& covarianceMatrix, const std::vector<Constraint>& constraints);

#endif // #ifndef DISPERSE_CONSTRAINT_UNPACKING
