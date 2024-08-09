
#include "Optimisation.hpp"

SafeCSC::SafeCSC(const SparseMatrix& matrix)
{
	SparseMatrix transposed = getTranspose(matrix);
	// std::map is ordered so it is already sorted by the key
	for (const auto column : transposed.matrixElements())
	{
		for (const auto cell : column.second)
		{
			while (columnPointers.size() <= column.first)
			{
				columnPointers.push_back(values.size());
			}
			rowIndices.push_back(cell.first);
			values.push_back(cell.second);
		}
	}
	while (columnPointers.size() <= matrix.columnCount())
	{
		columnPointers.push_back(rowIndices.size());
	}
	nzmax = rowIndices.size();
	m = matrix.rowCount();
	n = matrix.columnCount();
	p = columnPointers.data();
	i = rowIndices.data();
	x = values.data();
	nz = -1;
}

void WorkspaceDeleter::operator()(OSQPWorkspace* pOsqpWorkspace)
{
	if (pOsqpWorkspace)
	{
		osqp_cleanup(pOsqpWorkspace);
	}
}

OSQPSettings getSettings()
{
	OSQPSettings osqp_settings;
	osqp_set_default_settings(&osqp_settings);
	return osqp_settings;
}

void checkErrorStatus(OSQPErrorStatus errorStatus)
{
	switch (errorStatus)
	{
	case OSQPErrorStatus::SUCCESS:
		return;
	case OSQPErrorStatus::MEM_ALLOC_ERROR:
		throw InsufficientMemoryException();
	case OSQPErrorStatus::LINSYS_SOLVER_LOAD_ERROR:
	case OSQPErrorStatus::LINSYS_SOLVER_INIT_ERROR:
		throw SolverInitialisationException();
	case OSQPErrorStatus::DATA_VALIDATION_ERROR:
	case OSQPErrorStatus::SETTINGS_VALIDATION_ERROR:
	case OSQPErrorStatus::WORKSPACE_NOT_INIT_ERROR:
	case OSQPErrorStatus::NONCVX_ERROR:
	default:
		throw UnexpectedException();
	}
}

std::unique_ptr<OSQPWorkspace, WorkspaceDeleter> callOSQPSetup(OSQPData& osqpData)
{
	static const OSQPSettings osqp_settings = getSettings();
	OSQPWorkspace* pOsqpWorkspace = NULL;
	checkErrorStatus(static_cast<OSQPErrorStatus>(osqp_setup(&pOsqpWorkspace, &osqpData, &osqp_settings)));
	if (!pOsqpWorkspace)
	{
		throw UnexpectedException();
	}
	return std::unique_ptr<OSQPWorkspace, WorkspaceDeleter>(pOsqpWorkspace, WorkspaceDeleter());
}

std::unique_ptr<OSQPWorkspace, WorkspaceDeleter> callOSQPSetup(
	const SparseMatrix& matrixP,
	const SparseMatrix& matrixA,
	const std::vector<c_float>& vectorL,
	const std::vector<c_float>& vectorU)
{
	OSQPData osqpData;
	osqpData.n = matrixA.columnCount();
	osqpData.m = matrixA.rowCount();

	if (matrixP.columnCount() != osqpData.n
		|| vectorL.size() != osqpData.m
		|| vectorU.size() != osqpData.m)
	{
		throw UnexpectedException();
	}

	SafeCSC cscP(matrixP);
	osqpData.P = &cscP;

	SafeCSC cscA(matrixA);
	osqpData.A = &cscA;

	// q is not needed so a zero-filled array is used
	std::vector<c_float> vectorQ(static_cast<unsigned int>(osqpData.n), 0.0);
	osqpData.q = vectorQ.data();

	osqpData.l = const_cast<c_float*>(vectorL.data());
	osqpData.u = const_cast<c_float*>(vectorU.data());

	// osqp_setup copies osqpData so it can then be freed
	return callOSQPSetup(osqpData);
}

std::vector<double> callOSQPSolve(OSQPWorkspace& osqpWorkspace)
{
	checkErrorStatus(static_cast<OSQPErrorStatus>(osqp_solve(&osqpWorkspace)));
	if (!osqpWorkspace.info)
	{
		throw UnexpectedException();
	}
	switch (static_cast<OSQPSolverStatus>(osqpWorkspace.info->status_val))
	{
	case OSQPSolverStatus::SOLVED_INACCURATE:
	case OSQPSolverStatus::MAX_ITER_REACHED:
		// Solution accepted in release mode but we must also test that results are acceptable in debug mode
#ifdef _DEBUG
		throw UnexpectedException();
#endif
	case OSQPSolverStatus::SOLVED:
		break;
	case OSQPSolverStatus::SIGINT:
		throw OptimisationInterruptedException();
	case OSQPSolverStatus::PRIMAL_INFEASIBLE:
	case OSQPSolverStatus::PRIMAL_INFEASIBLE_INACCURATE:
	case OSQPSolverStatus::DUAL_INFEASIBLE:
	case OSQPSolverStatus::DUAL_INFEASIBLE_INACCURATE:
	case OSQPSolverStatus::NON_CVX:
	case OSQPSolverStatus::UNSOLVED:
	default:
		throw UnexpectedException();
	}
	if (!osqpWorkspace.solution || !osqpWorkspace.solution->x)
	{
		throw UnexpectedException();
	}
	return std::vector<double>(osqpWorkspace.solution->x, osqpWorkspace.solution->x + osqpWorkspace.data->n);
}

std::vector<double> solve(const SparseMatrix& covarianceMatrix, const std::vector<Constraint>& constraints)
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

	std::unique_ptr<OSQPWorkspace, WorkspaceDeleter> osqpWorkspace = callOSQPSetup(covarianceMatrix, matrixA, vectorL, vectorU);
	return callOSQPSolve(*osqpWorkspace);
}
