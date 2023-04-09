#include "ExpectedException.hpp"
#include "Security.hpp"

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

SolverInitialisationException::SolverInitialisationException()
	: ExpectedException("The solver could not be initialised.", ReturnCode::SOLVER_INITIALISATION_EXCEPTION)
{
}

InsufficientMemoryException::InsufficientMemoryException()
	: ExpectedException("A memory allocation failed.", ReturnCode::INSUFFICIENT_MEMORY_EXCEPTION)
{
}

UnexpectedException::UnexpectedException()
	: ExpectedException("An unexpected error occured.", ReturnCode::UNEXPECTED_EXCEPTION)
{
}

OptimisationInterruptedException::OptimisationInterruptedException()
	: ExpectedException("The optimisation process was interrupted.", ReturnCode::OPTIMISATION_INTERRUPTED)
{
}

RequiredColumnNotFoundException::RequiredColumnNotFoundException(std::string columnName, std::string fileName)
	: ExpectedException("The required column '" + columnName + "' was not found in file '" + fileName + "'.", ReturnCode::REQUIRED_COLUMN_NOT_FOUND_EXCEPTION)
{
}

IdentifierNotRecognisedException::IdentifierNotRecognisedException(std::string identifier)
	: ExpectedException("The identifier '" + identifier + "' was not recognised.", ReturnCode::IDENTIFIER_NOT_RECOGNISED_EXCEPTION)
{
}

InvalidLimitSumException::InvalidLimitSumException()
	: ExpectedException("The sum of the maxima limits is invalid.", ReturnCode::INVALID_LIMIT_SUM)
{
}

InvalidLimitsException::InvalidLimitsException(const std::string& message)
	: ExpectedException(message, ReturnCode::INVALID_LIMITS)
{
}

InvalidCommandException::InvalidCommandException(const std::string& message)
	: ExpectedException(message, ReturnCode::INVALID_COMMAND)
{
}

ExcessiveSizeException::ExcessiveSizeException(const std::string& message)
	: ExpectedException(message, ReturnCode::EXCESSIVE_SIZE)
{
}

IncompatibleInputArgumentsException::IncompatibleInputArgumentsException(const std::string& message)
	: ExpectedException(message, ReturnCode::INCOMPATIBLE_INPUT_ARGUMENTS)
{
}

ExpectedException::ExpectedException(const std::string& errorMessage, const ReturnCode returnCode)
	: errorMessage(errorMessage), returnCode(returnCode)
{
}
