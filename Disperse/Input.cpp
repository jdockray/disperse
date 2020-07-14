
#include "Input.hpp"

std::vector<Security> inputSecurities(std::ifstream& inputFile)
{
	std::optional<unsigned int> securityIndentifierColumnIndex;
	std::optional<unsigned int> expectedReturnColumnIndex;
	std::optional<unsigned int> riskColumnIndex;
	std::optional<unsigned int> minimumAllocationColumnIndex;
	std::optional<unsigned int> maximumAllocationColumnIndex;

	std::vector<Security> securities;

	
	

	

	return securities;
}

void augmentFactors(std::vector<Security>& ssecurities, std::ifstream& inputFile)
{


}

std::vector<Security> inputSecurities(const std::string& inputFileName)
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

void augmentFactors(std::vector<Security>& ssecurities, const std::string& inputFileName)
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

