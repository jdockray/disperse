
#ifndef DISPERSE_OPTIMISATION
#define DISPERSE_OPTIMISATION

#include "Security.hpp"
#include "Group.hpp"
#include "Matrices.hpp"
#include "Constraint.hpp"

#pragma warning(push, 0)
#include "dlib\matrix.h"
#include "osqp\include\osqp.h"
#include <vector>
#include <map>
#pragma warning(pop)

enum class OSQPErrorStatus
{
	SUCCESS = 0,
	DATA_VALIDATION_ERROR = OSQP_DATA_VALIDATION_ERROR,
	SETTINGS_VALIDATION_ERROR = OSQP_SETTINGS_VALIDATION_ERROR,
	LINSYS_SOLVER_LOAD_ERROR = OSQP_LINSYS_SOLVER_LOAD_ERROR,
	LINSYS_SOLVER_INIT_ERROR = OSQP_LINSYS_SOLVER_INIT_ERROR,
	NONCVX_ERROR = OSQP_NONCVX_ERROR,
	MEM_ALLOC_ERROR = OSQP_MEM_ALLOC_ERROR,
	WORKSPACE_NOT_INIT_ERROR = OSQP_WORKSPACE_NOT_INIT_ERROR
};

enum class OSQPSolverStatus
{
	SOLVED = OSQP_SOLVED,
	SOLVED_INACCURATE = OSQP_SOLVED_INACCURATE,
	MAX_ITER_REACHED = OSQP_MAX_ITER_REACHED,
	PRIMAL_INFEASIBLE = OSQP_PRIMAL_INFEASIBLE,
	PRIMAL_INFEASIBLE_INACCURATE = OSQP_PRIMAL_INFEASIBLE_INACCURATE,
	DUAL_INFEASIBLE = OSQP_DUAL_INFEASIBLE,
	DUAL_INFEASIBLE_INACCURATE = OSQP_DUAL_INFEASIBLE_INACCURATE,
	SIGINT = OSQP_SIGINT,
	UNSOLVED = OSQP_UNSOLVED,
	NON_CVX = OSQP_NON_CVX
};

class SafeCSC : public csc
{
public:
	SafeCSC(const SparseMatrix& sparseMatrix);

private:
	SafeCSC(SafeCSC const&) = delete;
	SafeCSC& operator=(SafeCSC const&) = delete;

	std::vector<c_int> columnPointers;
	std::vector<c_int> rowIndices;
	std::vector<c_float> values;
};

struct WorkspaceDeleter
{
	void operator()(OSQPWorkspace* pOsqpWorkspace);
};

std::vector<double> solve(const SparseMatrix& covarianceMatrix, const std::vector<Constraint>& constraints);

#endif // #ifndef DISPERSE_OPTIMISATION