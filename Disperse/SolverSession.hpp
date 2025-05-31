
#ifndef DISPERSE_SOLVER_SESSION
#define DISPERSE_SOLVER_SESSION

#include "Exceptions.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include "osqp\osqp\include\osqp.h"
#include <vector>
#pragma warning(pop)

class OSQPSession {
public:
	OSQPSession(OSQPData& osqpData, const OSQPSettings& osqp_settings);
	std::vector<double> callOSQPSolve();
	~OSQPSession();

private:
	OSQPWorkspace* pOsqpWorkspace;
};

#endif // #ifndef DISPERSE_SOLVER_SESSION
