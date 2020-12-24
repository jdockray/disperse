
#include "Input.hpp"

void trim(std::string& stringToTrim)
{
	stringToTrim.resize(stringToTrim.find_last_not_of("\r\n"));
}

std::vector<std::string> split(const std::string& stringToSplit, const char delimiter)
{
	std::stringstream stringStream(stringToSplit);
	std::vector<std::string> terms;
	std::string term;
	while (std::getline(stringStream, term, delimiter))
	{
		terms.push_back(term);
	}
	return terms;
}

std::vector<std::string> getLineElements(std::ifstream& inputFile)
{
	std::string line;
	std::getline(inputFile, line);
	trim(line);
	split(line, ',');

}

std::vector<Security> inputSecurities(std::ifstream& inputFile)
{
	std::optional<unsigned int> securityIndentifierColumnIndex;
	std::optional<unsigned int> expectedReturnColumnIndex;
	std::optional<unsigned int> riskColumnIndex;
	std::optional<unsigned int> minimumAllocationColumnIndex;
	std::optional<unsigned int> maximumAllocationColumnIndex;


	

	
	

	

	return securities;
}

void augmentFactors(std::map<std::string, Security>& securities, std::ifstream& inputFile)
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

void augmentFactors(std::map<std::string, Security> & securities, const std::string& inputFileName)
{
	std::ifstream inputFile;
	try
	{
		inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		inputFile.open(inputFileName, std::ios_base::in);
		augmentFactors(securities, inputFile);
	}
	catch (std::ios_base::failure)
	{
		throw InputFileException(inputFileName);
	}
}

