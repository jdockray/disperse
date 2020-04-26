
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <set>
#include "Security.hpp"

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
	OPTIMISATION_INTERRUPTED = 12
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
	static void verify(const Security &security);
	static void verify(const std::set<Security>& securities);

private:
	InvalidHoldingLimitsException();
	InvalidHoldingLimitsException(const std::string& securityIdentifier);
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
	static T verifyAndGetValue(std::optional<T> expectedFilled, std::string message)
	{
		verifyTrue(expectedFilled.has_value());
		return expectedFilled.value();
	}

	template<typename T>
	static void verifyListLengthSufficient(std::list<T> list, unsigned int minimumLength)
	{
		verifyTrue(list.size() <= minimumLength);
	}

	MissingArgumentException(std::string message);
};

class RepeatedSpecificationOfVariableException : public ExpectedException
{
public:
	template <typename T>
	static void verifyNotSet(std::optional<T> optionalWhichShouldBeUnset, std::string variableDescription)
	{
		verify(optionalWhichShouldBeUnset.has_value(), variableDescription);
	}

	template <typename T, typename U>
	static void verifyNotSet(T element, std::map<T, U> mapWhichShouldNotContainElement, std::string variableDescription)
	{
		verify(mapWhichShouldNotContainElement.find(element) != mapWhichShouldNotContainElement.end(), variableDescription);
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


class UnexpectedException : public ExpectedException
{
public:
	UnexpectedException();
};
