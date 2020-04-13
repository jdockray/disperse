#include "ExpectedException.hpp"

double CouldNotParseNumberException::convert(const std::string &stringToParse)
{
	try
	{
		return std::stof(stringToParse);
	}
	catch (std::invalid_argument ex)
	{
		throw CouldNotParseNumberException(stringToParse);
	}
	catch (std::out_of_range ex)
	{
		throw CouldNotParseNumberException(stringToParse);
	}

}

CouldNotParseNumberException::CouldNotParseNumberException(const std::string& attemptedToParse)
	: ExpectedException("The string " + attemptedToParse + " could not be converted to a number.", ReturnCode::COULD_NOT_PARSE_NUMBER_EXCEPTION)
{
}

NegativeRiskException::NegativeRiskException(const double risk, const std::string& securityIdentifier)
	: ExpectedException("The security " + securityIdentifier + " has a negative risk of " + std::to_string(risk) + " given.", ReturnCode::NEGATIVE_RISK_EXCEPTION)
{
}

void NegativeRiskException::verify(const double risk, const std::string& securityIdentifier)
{
	if (risk < 0)
	{
		throw NegativeRiskException(risk, securityIdentifier);
	}
}

NegativeRiskException::NegativeRiskException(const double risk, const std::string& securityIdentifier)
	: ExpectedException("The security " + securityIdentifier + " has a negative risk of " + std::to_string(risk) + " given.", ReturnCode::NEGATIVE_RISK_EXCEPTION)
{
}

void InvalidHoldingLimitsException::verify(const Security &security)
{
	if (security.getMaxProportion() < security.getMinProportion())
	{
		throw InvalidHoldingLimitsException("The holding maximum is less than the holding minimum for " + security.identifier + ".");
	}
	if (security.getMinProportion() < 0)
	{
		throw InvalidHoldingLimitsException("The amount which can be placed in " + security.identifier + " is negative.");
	}
	if (security.getMinProportion() > 1)
	{
		throw InvalidHoldingLimitsException("The amount which can be placed in " + security.identifier + " is greater than 100%.");
	}
}

void InvalidHoldingLimitsException::verify(const std::set<Security>& securities)
{
	double sumOfMaxima = 0;
	double sumOfMinima = 0;
	for (Security security : securities)
	{
		verify(security);
		sumOfMaxima += security.getMaxProportion();
		sumOfMinima += security.getMinProportion();
	}
	if (sumOfMaxima < 1)
	{
		throw InvalidHoldingLimitsException("The maximum proportions of each security allowed add up to less than 100%");
	}
	if (sumOfMaxima > 1)
	{
		throw InvalidHoldingLimitsException("The minimum proportions of each security allowed add up to more than 100%");
	}
}

InvalidHoldingLimitsException::InvalidHoldingLimitsException(const std::string& securityIdentifier)
	: ExpectedException("The combination of holding limits is invalid.", ReturnCode::INVALID_HOLDING_LIMITS)
{
}

InvalidHoldingLimitsException::InvalidHoldingLimitsException(const std::string& message)
	: ExpectedException(message, ReturnCode::INVALID_HOLDING_LIMITS)
{
}

IOException::IOException()
	: ExpectedException("A file input/output error occurred.", ReturnCode::IO_EXCEPTION)
{
}

InputFileException::InputFileException(const std::string& inputFileName)
	: ExpectedException(inputFileName + " could not be opened for read.", ReturnCode::INPUT_FILE_EXCEPTION)
{
}

OutputFileException::OutputFileException(const std::string& outputFileName)
	: ExpectedException(outputFileName + " could not be opened for output.", ReturnCode::OUTPUT_FILE_EXCEPTION)
{
}

void MissingArgumentException::verifyTrue(bool expectedTrue, std::string message)
{
	if (!expectedTrue) throw MissingArgumentException(message);
}

MissingArgumentException::MissingArgumentException(std::string message)
	: ExpectedException(message, ReturnCode::MISSING_ARGUMENT_EXCEPTION)
{
}

void RepeatedSpecificationOfVariableException::verifyNotSet(bool presence, std::string variableDescription)
{
	if (presence)
	{
		throw RepeatedSpecificationOfVariableException(variableDescription);
	}
}

RepeatedSpecificationOfVariableException::RepeatedSpecificationOfVariableException(std::string variableDescription)
	: ExpectedException("Repeated specification of variable " + variableDescription, ReturnCode::REPEATED_SPECIFICATION_OF_VARIABLE)
{
}

ExpectedException::ExpectedException(const std::string &errorMessage, const ReturnCode returnCode)
	: errorMessage(errorMessage), returnCode(returnCode)
{
}


