
#include "SolverSession.hpp"

enum class OSQPSolverStatus {
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

enum class OSQPErrorStatus {
	SUCCESS = 0,
	DATA_VALIDATION_ERROR = OSQP_DATA_VALIDATION_ERROR,
	SETTINGS_VALIDATION_ERROR = OSQP_SETTINGS_VALIDATION_ERROR,
	LINSYS_SOLVER_LOAD_ERROR = OSQP_LINSYS_SOLVER_LOAD_ERROR,
	LINSYS_SOLVER_INIT_ERROR = OSQP_LINSYS_SOLVER_INIT_ERROR,
	NONCVX_ERROR = OSQP_NONCVX_ERROR,
	MEM_ALLOC_ERROR = OSQP_MEM_ALLOC_ERROR,
	WORKSPACE_NOT_INIT_ERROR = OSQP_WORKSPACE_NOT_INIT_ERROR
};

void checkErrorStatus(OSQPErrorStatus errorStatus) {
	switch (errorStatus) {
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

void checkSolverStatus(OSQPSolverStatus status) {
	switch (status) {
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
}

OSQPSession::OSQPSession(OSQPData& osqpData, const OSQPSettings& osqp_settings) {
	checkErrorStatus(static_cast<OSQPErrorStatus>(osqp_setup(&pOsqpWorkspace, &osqpData, &osqp_settings)));
	if (!pOsqpWorkspace) {
		throw UnexpectedException();
	}
}

std::vector<double> OSQPSession::callOSQPSolve() {
	checkErrorStatus(static_cast<OSQPErrorStatus>(osqp_solve(pOsqpWorkspace)));
	if (!pOsqpWorkspace->info) {
		throw UnexpectedException();
	}
	checkSolverStatus(static_cast<OSQPSolverStatus>(pOsqpWorkspace->info->status_val));
	if (!pOsqpWorkspace->solution || !pOsqpWorkspace->solution->x) {
		throw UnexpectedException();
	}
	return std::vector<double>(pOsqpWorkspace->solution->x, pOsqpWorkspace->solution->x + pOsqpWorkspace->data->n);
}
