
#ifndef DISPERSE_EXPECTED_EXCEPTION
#define DISPERSE_EXPECTED_EXCEPTION

#pragma warning(push, 0)
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <set>
#include <list>
#include <map>
#include <optional>
#pragma warning(pop)

enum class ReturnCode
{
	SUCCESS = 0,
	UNEXPECTED_EXCEPTION = 1,
	MISSING_ARGUMENT_EXCEPTION = 2,
	INPUT_FILE_EXCEPTION = 3,
	OUTPUT_FILE_EXCEPTION = 4,
	IO_EXCEPTION = 5,
	NEGATIVE_RISK_EXCEPTION = 6,
	INVALID_HOLDING_LIMITS = 7,
	REPEATED_SPECIFICATION_OF_VARIABLE = 8,
	COULD_NOT_PARSE_NUMBER_EXCEPTION = 9,
	SOLVER_INITIALISATION_EXCEPTION = 10,
	INSUFFICIENT_MEMORY_EXCEPTION = 11,
	OPTIMISATION_INTERRUPTED = 12,
	REQUIRED_COLUMN_NOT_FOUND_EXCEPTION = 13,
	IDENTIFIER_NOT_RECOGNISED_EXCEPTION = 14,
	INVALID_LIMIT_SUM = 15,
	INVALID_LIMITS = 16
};

struct ExpectedException : public std::exception
{
	ExpectedException(const std::string& errorMessage, const ReturnCode returnCode);

	const std::string errorMessage;
	const ReturnCode returnCode;
};

class CouldNotParseNumberException : public ExpectedException
{
public:
	static double convert(const std::string& stringToParse);

private:
	CouldNotParseNumberException(const std::string& attemptedToParse);
};

class NegativeRiskException : public ExpectedException
{
public:
	static void verify(const double risk, const std::string& securityIdentifier);

private:
	NegativeRiskException(const double risk, const std::string& securityIdentifier);
};

class InvalidHoldingLimitsException : public ExpectedException
{
public:
	InvalidHoldingLimitsException(const std::string& message);
};

class IOException : public ExpectedException
{
public:
	IOException();
};

class InputFileException : public ExpectedException
{
public:
	InputFileException(const std::string& inputFileName);
};

class OutputFileException : public ExpectedException
{
public:
	OutputFileException(const std::string& outputFileName);
};

class MissingArgumentException : public ExpectedException
{
public:
	static void verifyTrue(bool expectedTrue, std::string message);

	template<typename T>
	static T verifyAndGetValue(const std::optional<T>& expectedFilled, const std::string message)
	{
		verifyTrue(expectedFilled.has_value(), message);
		return expectedFilled.value();
	}

	template<typename T>
	static void verifyListLengthSufficient(const std::list<T> list, const unsigned int minimumLength,
		const std::string message)
	{
		verifyTrue(list.size() <= minimumLength, message);
	}

	MissingArgumentException(std::string message);
};

class RepeatedSpecificationOfVariableException : public ExpectedException
{
public:
	template <typename T>
	static void verifyNotSet(const std::optional<T>& optionalWhichShouldBeUnset, const std::string& variableDescription)
	{
		verifyNotSet(optionalWhichShouldBeUnset.has_value(), variableDescription);
	}

	template <typename T, typename U>
	static void verifyNotSet(
		const T element,
		const std::map<T, U> mapWhichShouldNotContainElement,
		const std::string variableDescription)
	{
		verifyNotSet(mapWhichShouldNotContainElement.find(element) != mapWhichShouldNotContainElement.end(), variableDescription);
	}

private:
	static void verifyNotSet(bool presence, std::string variableDescription);
	RepeatedSpecificationOfVariableException(std::string variableDescription);
};

class SolverInitialisationException : public ExpectedException
{
public:
	SolverInitialisationException();
};

class InsufficientMemoryException : public ExpectedException
{
public:
	InsufficientMemoryException();
};

class OptimisationInterruptedException : public ExpectedException
{
public:
	OptimisationInterruptedException();
};

class RequiredColumnNotFoundException : public ExpectedException
{
public:
	RequiredColumnNotFoundException(std::string columnName, std::string fileName);
};

class IdentifierNotRecognisedException : public ExpectedException
{
public:
	IdentifierNotRecognisedException(std::string identifier);
};

class InvalidLimitSumException : public ExpectedException
{
public:
	InvalidLimitSumException();
};

class InvalidLimitsException : public ExpectedException
{
public:
	InvalidLimitsException(const std::string& message);
};

class UnexpectedException : public ExpectedException
{
public:
	UnexpectedException();
};

#endif // #ifndef DISPERSE_EXPECTED_EXCEPTION
