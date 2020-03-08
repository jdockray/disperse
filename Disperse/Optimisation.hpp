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

// The limit is the maximum allowed risk or minimum allowed return
void solve(Objective objective, double limit, const std::vector<Security>& securities, dlib::matrix<double> covarianceMatrix)
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

	OSQPWorkspace* pOsqpWorkspace;
	ExitStatus exitStatus = static_cast<ExitStatus>(osqp_setup(&pOsqpWorkspace, &osqpData, &DISPERSE_OSQP_SETTINGS));


}