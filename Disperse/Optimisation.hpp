#pragma once
#include "dlib\matrix.h"
#include "osqp\include\osqp.h"
#include <vector>

enum class Objective
{
	MAXIMISE_RETURN,
	MINIMISE_RISK
};

enum class ExitStatus
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

// dlib::upperm can be used to make an upper triangular matrix
class SafeCSC : public csc
{
public:
	SafeCSC(dlib::matrix<double> matrix)
	{
		columnPointers.push_back(0);
		for (int i = 0; i < matrix.nr; ++i)
		{			
			for (int j = 0; j < matrix.nc; ++j)
			{
				double value = matrix(i, j);
				if (value > 0)
				{
					rowIndices.push_back(j);
					values.push_back(value);
				}
			}
			columnPointers.push_back(rowIndices.size());
		}
		nzmax = rowIndices.size();
		m = matrix.nr;
		n = matrix.nc;
		p = columnPointers.data();
		i = rowIndices.data(); c_int* i;
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

// The limit is the maximum allowed risk or minimum allowed return
void solve(Objective objective, double limit, const std::vector<Security>& securities, dlib::matrix<double> covarianceMatrix)
{
	std::vector<c_float> qVector(securities.size());
	std::vector<unsigned int> indicesOfSecuritiesWithLimits;
	std::vector<c_float> lVector;
	std::vector<c_float> uVector;
	for (int i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.at(i);
		// Code to set an individual value of Q required here
		if (security.getMinProportion() > 0 && security.getMaxProportion())
		{
			indicesOfSecuritiesWithLimits.push_back(i);
			lVector.push_back(security.getMinProportion());
			uVector.push_back(security.getMaxProportion());
		}

	}
	
	dlib::matrix<double> aMatrix(osqpData.m, osqpData.n);

	SafeCSC safeCSC(covarianceMatrix);
	OSQPData osqpData;
	osqpData.n = securities.size();

	c_int    m; ///< number of constraints m
	osqpData.P = &safeCSC; /// the upper triangular part of the quadratic cost matrix P in csc format (size n x n).



		csc* A; ///< linear constraints matrix A in csc format (size m x n)
		c_float* q; ///< dense array for linear part of cost function (size n)
		c_float* l; ///< dense array for lower bound (size m)
		c_float* u; ///< dense array for upper bound (size m)
	

	OSQPWorkspace* pOsqpWorkspace;
	ExitStatus exitStatus = static_cast<ExitStatus>(osqp_setup(&pOsqpWorkspace, &osqpData, &DISPERSE_OSQP_SETTINGS));


}