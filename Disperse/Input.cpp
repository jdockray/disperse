
#include "Input.hpp"

InputData inputSecurities(std::ifstream& inputFile)
{
	std::optional<unsigned int> securityIndentifierColumnIndex;
	std::optional<unsigned int> expectedReturnColumnIndex;
	std::optional<unsigned int> riskColumnIndex;
	std::optional<unsigned int> minimumAllocationColumnIndex;
	std::optional<unsigned int> maximumAllocationColumnIndex;

	std::vector<Security> securities;
	std::vector<std::vector<double> > correlations;


	InputData inputData(securities);
	
	for (auto correlationSet : correlations)
	{
		// We need to check that all of the vectors in this vector of vectors have the same size


	}

	// Multiplication is required to get from factor correlations to the covariance matrix

	return inputData;
}

void augmentFactors(InputData& inputData, std::ifstream& inputFile)
{


}

InputData inputSecurities(const std::string& inputFileName)
{
	std::ifstream inputFile;
	try
	{
		inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		inputFile.open(inputFileName, std::ios_base::in);
		return inputSecurities(inputFile);
	}
	catch (std::ios_base::failure)
	{
		throw InputFileException(inputFileName);
	}
}

void augmentFactors(InputData& inputData, const std::string& inputFileName)
{
	std::ifstream inputFile;
	try
	{
		inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		inputFile.open(inputFileName, std::ios_base::in);
		augmentFactors(inputData, inputFile);
	}
	catch (std::ios_base::failure)
	{
		throw InputFileException(inputFileName);
	}
}

