
#ifndef DISPERSE_INPUT
#define DISPERSE_INPUT

#include "Security.hpp"
#include "Matrices.hpp"
#include "dlib\matrix.h"
#include <vector>

class InputData
{
public:
	InputData(const std::vector<Security>& securities)
		: securitiesList(securities), covarianceMatrix(securities.size())
	{
	}

	const std::vector<Security>& securities() const
	{
		return securitiesList;
	}

	UpperTriangularSparseMatrix& covariances()
	{
		return covarianceMatrix;
	}

private:
	const std::vector<Security> securitiesList;
	UpperTriangularSparseMatrix covarianceMatrix;	
};

InputData inputSecurities(const std::string& inputFileName);

#endif // #ifndef DISPERSE_INPUT
