
#ifndef DISPERSE_INPUT
#define DISPERSE_INPUT

#include "Security.hpp"
#include "dlib\matrix.h"
#include <vector>

void inputSecurities(const std::string& inputFileName, const std::vector<Security>& securitiesListToAddTo,
	dlib::matrix<double>& covarianceMatrixToAddTo);

void inputSecurities(std::ifstream& inputFile, const std::vector<Security>& securitiesListToAddTo,
	dlib::matrix<double>& covarianceMatrixToAddTo);


#endif // #ifndef DISPERSE_INPUT
