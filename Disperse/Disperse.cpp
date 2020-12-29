
#include "Input.hpp"
#include "Output.hpp"
#include "ExpectedException.hpp"
#include "CmdLine.hpp"
#include "Optimisation.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

template <class T, class U>
std::map<U, size_t> generateMappingFromIterable(const T& items)
{
	size_t index = 0;
	std::map<U, size_t> mapping;
	for (const auto& item : items)
	{
		mapping[item] = index++;
	}
	return mapping;
}

SparseMatrix generateFactorMatrix(const ListOfSecurities& securities, const std::vector<std::string> factors)
{
	std::map<std::string, size_t> factorMapping = generateMappingFromIterable<std::vector<std::string>, std::string>(factors);
	SparseMatrix factorMatrix(securities.size(), factorMapping.size());
	for (unsigned int i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.getSecurity(i);
		for (auto exposure : security.getExposures())
		{
			factorMatrix.setValue(i, factorMapping.at(exposure.first), exposure.second);
		}
	}
	return factorMatrix;
}

std::vector<double> getSecurityRisks(const ListOfSecurities& securities)
{
	std::vector<double> risks;
	for (const auto& security : securities.getSecurities())
	{
		risks.push_back(security.getRisk());
	}
	return risks;
}

void run(const std::string& inputFileName,
	const std::string& securityOutputFileName,
	const double minimumReturn,
	const std::optional<std::string> factorGridFileName = std::optional<std::string>(),
	const std::optional<std::string> factorListFileName = std::optional<std::string>(),
	const std::optional<std::string> factorOutputFileName = std::optional<std::string>())
{
	ListOfSecurities securities = inputSecurities(inputFileName);
	if (factorGridFileName.has_value())
	{
		inputFactorGrid(factorGridFileName.value(), securities);
	}
	if (factorListFileName.has_value())
	{
		inputFactorList(factorListFileName.value(), securities);
	}
	std::vector<std::string> factors = securities.getAllFactors();
	const SparseMatrix factorMatrix = generateFactorMatrix(securities, factors);
	UpperTriangularSparseMatrix correlationMatrix
		= multiply<UpperTriangularCorrelationMatrix>(factorMatrix.getTranspose(), factorMatrix);
	DiagonalSparseMatrix riskDiagonalMatrix(getSecurityRisks(securities));
	const UpperTriangularSparseMatrix covarianceMatrix = multiply<UpperTriangularSparseMatrix>(
		multiply<UpperTriangularSparseMatrix>(riskDiagonalMatrix, correlationMatrix),
		riskDiagonalMatrix
	);
	Solution solution = solve(minimumReturn, securities, covarianceMatrix);
	outputAllocations(securities.getIdentifiers(), solution.m_securityProportions, securityOutputFileName);
	if (factorOutputFileName.has_value())
	{
		outputFactorExposures(factors, solution.m_factorProportions, factorOutputFileName.value());
	}
}

void run(const unsigned int argc, const char* const argv[])
{
	CmdLineArgs cmdLineArgs(argc, argv);
	const std::optional<std::string> factorGridFileName = cmdLineArgs.getSingleArgumentOption('g');
	const std::optional<std::string> factorListFileName = cmdLineArgs.getSingleArgumentOption('l');
	const std::optional<std::string> factorOutputFileName = cmdLineArgs.getSingleArgumentOption('f');
	const std::list<std::string>& requiredArgs = cmdLineArgs.remainingArguments();
	MissingArgumentException::verifyListLengthSufficient(requiredArgs, 3, "There are not enough positional command line arguments.");
	std::list<std::string>::const_iterator position = requiredArgs.begin();
	std::string inputFileName = *position;
	std::string securityOutputFileName = *(++position);
	double minimumReturn = CouldNotParseNumberException::convert(*(++position));
	run(inputFileName, securityOutputFileName, minimumReturn,
		factorGridFileName,	factorListFileName,	factorOutputFileName);
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
