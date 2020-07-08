
#include "Input.hpp"

void inputSecurities(std::ifstream& inputFile, const std::vector<Security>& securitiesListToAddTo,
	dlib::matrix<double>& covarianceMatrixToAddTo)
{

}

void inputSecurities(const std::string& inputFileName, const std::vector<Security>& securitiesListToAddTo,
	dlib::matrix<double>& covarianceMatrixToAddTo)
{
	std::ifstream inputFile;
	try
	{
		inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		inputFile.open(inputFileName, std::ios_base::in);
		inputSecurities(inputFile, securitiesListToAddTo, covarianceMatrixToAddTo);
	}
	catch (std::ios_base::failure)
	{
		throw InputFileException(inputFileName);
	}
}

