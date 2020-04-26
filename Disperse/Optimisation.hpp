

#ifndef DISPERSE_OPTIMISATION
#define DISPERSE_OPTIMISATION

#include "dlib\matrix.h"
#include "osqp\include\osqp.h"
#include <vector>

enum class SetupExitStatus
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

enum class SolveExitStatus
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

struct SparseMatrixElement
{
	SparseMatrixElement(c_int rowIndex, c_int columnIndex, c_float value)
		: rowIndex(rowIndex), columnIndex(columnIndex), value(value)
	{
	}

	c_int rowIndex;
	c_int columnIndex;
	c_float value;
};

bool operator<(const SparseMatrixElement& a, const SparseMatrixElement& b)
{
	if (a.columnIndex != b.columnIndex) return a.columnIndex < b.columnIndex;
	return a.rowIndex < b.rowIndex;
}

SafeCSC safeCSCFromDlibMatrixUpperTriangular(dlib::matrix<double> matrix)
{
	std::vector<SparseMatrixElement> vectorOfSparseElements;
	for (int i = 0; i < matrix.nr; ++i)
	{
		for (int j = i; j < matrix.nc; ++j)
		{
			vectorOfSparseElements.push_back(SparseMatrixElement(i, j, matrix(i, j)));
		}
	}
	return SafeCSC(vectorOfSparseElements, matrix.nr, matrix.nc);
}

// dlib::upperm can be used to make an upper triangular matrix
class SafeCSC : public csc
{
public:
	SafeCSC(const std::vector<SparseMatrixElement> &vectorOfSparseElements,
		c_int matrixRowNumber, c_int matrixColumnNumber)
	{
		std::sort(vectorOfSparseElements.front(), vectorOfSparseElements.back());
		c_int currentColumn = -1;
		for (SparseMatrixElement element : vectorOfSparseElements)
		{
			if (element.columnIndex > currentColumn) columnPointers.push_back(rowIndices.size());
			rowIndices.push_back(element.rowIndex);
			values.push_back(element.value);
		}
		nzmax = rowIndices.size();
		m = matrixRowNumber;
		n = matrixColumnNumber;
		p = columnPointers.data();
		i = rowIndices.data();
		x = values.data();
		nz = -1;
	}

private:
	std::vector<c_int> columnPointers;
	std::vector<c_int> rowIndices;
	std::vector<c_float> values;
};

static const class DisperseOSQPSettings : public OSQPSettings
{
	DisperseOSQPSettings()
	{
		osqp_set_default_settings(this);
	}

} DISPERSE_OSQP_SETTINGS;


class ScopedOSQPWorkspaceUsage
{
public:
	ScopedOSQPWorkspaceUsage(OSQPWorkspace& workspace)
		: m_workspace(workspace)
	{
	}
	
	OSQPWorkspace &getWorkspace()
	{
		return m_workspace;
	}

	~ScopedOSQPWorkspaceUsage()
	{
		osqp_cleanup(&m_workspace);
	}

private:
	OSQPWorkspace& m_workspace;
};
	
OSQPWorkspace& callOSQPSetup(OSQPData &osqpData)
{
	OSQPWorkspace* pOsqpWorkspace = NULL;
	switch (static_cast<SetupExitStatus>(osqp_setup(&pOsqpWorkspace, &osqpData, &DISPERSE_OSQP_SETTINGS)))
	{
	case SetupExitStatus::SUCCESS:
		break;
	case SetupExitStatus::MEM_ALLOC_ERROR:
		throw InsufficientMemoryException();
	case SetupExitStatus::LINSYS_SOLVER_LOAD_ERROR:
	case SetupExitStatus::LINSYS_SOLVER_INIT_ERROR:
		throw SolverInitialisationException();
	case SetupExitStatus::DATA_VALIDATION_ERROR:
	case SetupExitStatus::SETTINGS_VALIDATION_ERROR:
	case SetupExitStatus::WORKSPACE_NOT_INIT_ERROR:
	case SetupExitStatus::NONCVX_ERROR:
	default:
		throw UnexpectedException();
	}
	if (!pOsqpWorkspace)
	{
		throw UnexpectedException();
	}
	return *pOsqpWorkspace;
}

void callOSQPSolve(ScopedOSQPWorkspaceUsage& workspaceUsage)
{
	switch (static_cast<SolveExitStatus>(osqp_solve(&workspaceUsage.getWorkspace())))
	{
	case SolveExitStatus::SOLVED_INACCURATE:
	case SolveExitStatus::MAX_ITER_REACHED:
// Solution accepted in release mode but we must also test that results are acceptable in debug mode
#ifndef _DEBUG
		throw UnexpectedException();
#endif
	case SolveExitStatus::SOLVED:
		return;
	case SolveExitStatus::SIGINT:
		throw OptimisationInterruptedException();
	case SolveExitStatus::PRIMAL_INFEASIBLE:
	case SolveExitStatus::PRIMAL_INFEASIBLE_INACCURATE:
	case SolveExitStatus::DUAL_INFEASIBLE:
	case SolveExitStatus::DUAL_INFEASIBLE_INACCURATE:
	case SolveExitStatus::NON_CVX:
	case SolveExitStatus::UNSOLVED:
	default:
		throw UnexpectedException();
	}
}

void solve(double minimumReturn, const std::vector<Security>& securities, dlib::matrix<double> covarianceMatrix)
{
	std::vector<c_float> qVector(securities.size());
	std::vector<unsigned int> indicesOfSecuritiesWithLimits;
	std::vector<c_float> lVector;
	lVector.push_back(0);
	std::vector<c_float> uVector;
	uVector.push_back(1);
	std::vector<SparseMatrixElement> elementsOfA;
	for (int i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.at(i);
		elementsOfA.push_back(SparseMatrixElement(i, 0, 1)); // For sum of all allocations
		if (security.getMinProportion() > 0 && security.getMaxProportion())
		{
			indicesOfSecuritiesWithLimits.push_back(i);
			lVector.push_back(security.getMinProportion());
			uVector.push_back(security.getMaxProportion());
			elementsOfA.push_back(SparseMatrixElement(i, i + 1, 1));
		}
	}

	OSQPData osqpData;
	osqpData.n = securities.size();
	osqpData.m = securities.size() + 1;

	SafeCSC cscP = safeCSCFromDlibMatrixUpperTriangular(covarianceMatrix);
	osqpData.P = &cscP;

	SafeCSC cscA(elementsOfA, osqpData.m, osqpData.n);
	osqpData.A = &cscA;

	osqpData.q = qVector.data();
	osqpData.l = lVector.data();
	osqpData.u = uVector.data();

	OSQPWorkspace osqpWorkspace = callOSQPSetup(osqpData);
	
	// Check pOsqpWorkspace not null, asserting and throwing if necessary, maybe in scoped usage constructor

	ScopedOSQPWorkspaceUsage scopedWorkspaceUsage(osqpWorkspace);

	callOSQPSolve(scopedWorkspaceUsage);




}

#endif // #ifndef DISPERSE_OPTIMISATION