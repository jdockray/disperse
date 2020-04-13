
#include "Output.hpp"
//#include <iostream>
#include <fstream>

std::ofstream openOutputFile(const std::string& fileName)
{
	std::ofstream outputFile;
	outputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try
	{
		outputFile.open(fileName, std::ios_base::out);
	}
	catch (std::ios_base::failure)
	{
		throw OutputFileException(fileName);
	}
}

void outputAllocation(const std::vector<std::pair<Security, double> >& allocations, std::string outputFileName)
{
	std::ofstream outputFile = openOutputFile(outputFileName);
	outputFile << "\"Asset\", \"Allocation\"" << std::endl;
	for (
		std::vector<std::pair<Security, double> >::const_iterator allocation = allocations.begin();
		allocation != allocations.end();
		++allocation
		)
	{
		outputFile << "\"" << allocation->first.identifier << "\", " << allocation->second << std::endl;
	}
	outputFile.close();
}