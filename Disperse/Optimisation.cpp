
#include "Optimisation.hpp"

SafeCSC::SafeCSC(const SparseMatrix& sparseMatrix)
{
	// std::map is ordered so it is already sorted by the key
	for (const std::pair<std::pair<dlib::uint32, dlib::uint32>, double>& entry : sparseMatrix.matrixElements())
	{
		while (columnPointers.size() <= entry.first.first)
		{
			columnPointers.push_back(rowIndices.size());
		}
		rowIndices.push_back(entry.first.second);
		values.push_back(entry.second);
	}
	while (columnPointers.size() <= sparseMatrix.columnCount())
	{
		columnPointers.push_back(rowIndices.size());
	}
	nzmax = rowIndices.size();
	m = sparseMatrix.rowCount();
	n = sparseMatrix.columnCount();
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
	const UpperTriangularSparseMatrix& matrixP,
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
	std::vector<c_float> vectorQ(osqpData.n, 0.0);
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

std::vector<double> solve(double minimumReturn, const ListOfSecurities& securities,
	const UpperTriangularSparseMatrix& covarianceMatrix)
{
	std::vector<c_float> vectorL;
	vectorL.push_back(1);
	std::vector<c_float> vectorU;
	vectorU.push_back(1);
	SparseMatrix matrixA(securities.numberOfConstrainedSecurities() + 1, securities.size());
	for (unsigned int i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.getSecurity(i);
		matrixA.setValue(0, i, 1); // For sum of all allocations
		if (security.hasConstrainedProportion())
		{
			//vectorQ.push_back(security.getExpectedReturn());
			vectorL.push_back(security.getMinProportion());
			vectorU.push_back(security.getMaxProportion());
			matrixA.setValue(i + 1, i, 1);
		}
	}

	std::unique_ptr<OSQPWorkspace, WorkspaceDeleter> osqpWorkspace = callOSQPSetup(covarianceMatrix, matrixA, vectorL, vectorU);

	return callOSQPSolve(*osqpWorkspace);
}
