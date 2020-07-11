
#include "Input.hpp"

enum class ColumnType
{
	SECURITY_IDENTIFIER,
	EXPECTED_RETURN,
	RISK,
	CORRELATIONS_AGAINST_FACTOR,
	FACTOR_DESCRIPTOR,
	CORRECTION_AGAINST_DESCRIBED_FACTOR
};

class ColumnHeader
{
public:
	const ColumnType columnType;

protected:
	ColumnHeader(const ColumnType type)
		: columnType(type)
	{
	}
};

class SecurityIdentifierColumnHeader : public ColumnHeader
{
public:
	SecurityIdentifierColumnHeader()
		: ColumnHeader(ColumnType::SECURITY_IDENTIFIER)
	{
	}
};

class ExpectedReturnColumnHeader : public ColumnHeader
{
public:
	ExpectedReturnColumnHeader()
		: ColumnHeader(ColumnType::EXPECTED_RETURN)
	{
	}
};

class RiskColumnHeader : public ColumnHeader
{
public:
	RiskColumnHeader()
		: ColumnHeader(ColumnType::RISK)
	{
	}
};

class CorrelationsAgainstFactorColumnHeader : public ColumnHeader
{
public:
	CorrelationsAgainstFactorColumnHeader(const unsigned int index)
		: ColumnHeader(ColumnType::CORRELATIONS_AGAINST_FACTOR), factorIndex(index)
	{
	}

	CorrelationsAgainstFactorColumnHeader(const unsigned int index, const unsigned int group)
		: ColumnHeader(ColumnType::CORRELATIONS_AGAINST_FACTOR), factorIndex(index), factorGroup(group)
	{
	}

	const unsigned int factorIndex;
	const std::optional<unsigned int> factorGroup;
};


InputData inputSecurities(std::ifstream& inputFile)
{
	std::vector<Security> securities;
	
	InputData inputData(securities);
	
	

	return inputData;
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

