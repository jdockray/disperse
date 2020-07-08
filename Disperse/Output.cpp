
#include "Output.hpp"
//#include <iostream>
#include <fstream>


void outputAllocations(const std::vector<Security> &securities, const std::vector<double>& allocations, std::ofstream& outputFile)
{
	unsigned int numberOfAllocations = securities.size();
	if (allocations.size() != numberOfAllocations)
	{
		throw UnexpectedException();
	}
	outputFile << "\"Asset\", \"Allocation\"" << std::endl;
	for (unsigned int i = 0; i < numberOfAllocations; i++)
	{
		outputFile << "\"" << securities.at(i).identifier << "\", " << allocations.at(i) << std::endl;
	}
}

void outputAllocations(const std::vector<Security>& securities, const std::vector<double>& allocations, const std::string& outputFileName)
{
	std::ofstream outputFile;
	try
	{
		outputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		outputFile.open(outputFileName, std::ios_base::out);
		outputAllocations(securities, allocations, outputFile);
	}
	catch (std::ios_base::failure)
	{
		throw OutputFileException(outputFileName);
	}
}
