
#include "Input.hpp"
#include "Output.hpp"
#include "ExpectedException.hpp"
#include "CmdLine.hpp"
#include "Optimisation.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

std::set<std::string> getAllFactors(std::vector<Security> securities)
{
	std::set<std::string> factors;
	for (auto security : securities)
	{
		for (auto exposures : security.getExposures())
		{
			factors.insert(exposures.first);
		}
	}
	return factors;
}

template <class T>
std::map<T, size_t> generateMappingFromSet(const std::set<T>& items)
{
	size_t index = 0;
	std::map<T, unsigned int> mapping;
	for (const auto& item : items)
	{
		mapping[item] = index++;
	}
	return mapping;
}

SparseMatrix generateFactorMatrix(std::vector<Security> securities)
{
	std::map<std::string, size_t> factorMapping = generateMappingFromSet<std::string>(getAllFactors(securities));
	SparseMatrix factorMatrix(securities.size(), factorMapping.size());
	for (int i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.at(i);
		for (auto exposure : security.getExposures())
		{
			factorMatrix.setValue(i, factorMapping.at(exposure.first), exposure.second);
		}
	}

}

std::vector<double> getSecurityRisks(const std::vector<Security>& securities)
{
	std::vector<double> risks;
	for (const auto& security : securities)
	{
		risks.push_back(security.getRisk());
	}
	return risks;
}

void run(const std::string& inputFileName, const std::string& outputFileName, const double minimumReturn, const std::optional<std::string&> additionalFactorsFileName = std::optional<std::string&>())
{
	std::vector<Security> securities = inputSecurities(inputFileName);
	if (additionalFactorsFileName.has_value())
	{
		augmentFactors(securities, additionalFactorsFileName.value());
	}
	const SparseMatrix factorMatrix = generateFactorMatrix(securities);
	UpperTriangularSparseMatrix correlationMatrix = multiply<UpperTriangularSparseMatrix>(factorMatrix.getTranspose(), factorMatrix);
	DiagonalSparseMatrix riskDiagonalMatrix(getSecurityRisks(securities));
	const UpperTriangularSparseMatrix covarianceMatrix = multiply<UpperTriangularSparseMatrix>(
		multiply<UpperTriangularSparseMatrix>(riskDiagonalMatrix, correlationMatrix),
		riskDiagonalMatrix
	);
	outputAllocations(securities, solve(minimumReturn, securities, covarianceMatrix), outputFileName);
}

void run(const unsigned int argc, const char* const argv[])
{
	CmdLineArgs cmdLineArgs(argc, argv);
	const std::list<std::string>& requiredArgs = cmdLineArgs.remainingArguments();
	MissingArgumentException::verifyListLengthSufficient(requiredArgs, 3);
	std::list<std::string>::const_iterator position = requiredArgs.begin();
	std::string inputFileName = *position;
	std::string outputFileName = *(++position);
	double minimumReturn = CouldNotParseNumberException::convert(*(++position));
	run(inputFileName, outputFileName, minimumReturn);
}

void runCatchingGlobalExceptions(const unsigned int argc, const char* const argv[])
{
	try
	{
		run(argc, argv);
	}
	catch (std::bad_alloc)
	{
		throw InsufficientMemoryException();
	}
}

int main(int argc, char* argv[])
{
	ReturnCode returnCode = ReturnCode::UNEXPECTED_EXCEPTION;
	try
	{
		runCatchingGlobalExceptions(argc, argv);
		returnCode = ReturnCode::SUCCESS;
	}
	catch (ExpectedException ex)
	{
		std::cerr << ex.errorMessage;
		returnCode = ex.returnCode;
	}
#ifndef _DEBUG
	catch (...)
	{
		UnexpectedException unexpectedException;
		std::cerr << unexpectedException.errorMessage;
	}
#endif
	return static_cast<int>(returnCode);
}
