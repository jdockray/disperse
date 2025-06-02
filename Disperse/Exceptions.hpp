
#ifndef DISPERSE_EXCEPTIONS
#define DISPERSE_EXCEPTIONS

#pragma warning(push, 0)
#include "csvstream\csvstream\csvstream.hpp"
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>
#include <set>
#include <list>
#include <map>
#include <optional>
#pragma warning(pop)

enum class ReturnCode {
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
	INVALID_LIMITS = 16,
	INVALID_COMMAND = 17,
	EXCESSIVE_SIZE = 18,
	INCOMPATIBLE_INPUT_ARGUMENTS = 19,
	CSV_STREAM_EXCEPTION = 20
};

struct ExpectedException : public std::exception {
	ExpectedException(const std::string& errorMessage, const ReturnCode returnCode);

	const std::string errorMessage;
	const ReturnCode returnCode;
};

class CouldNotParseNumberException : public ExpectedException {
public:
	CouldNotParseNumberException(const std::string& attemptedToParseVariable, const std::string& context);
};

class NegativeRiskException : public ExpectedException {
public:
	NegativeRiskException(const double risk, const std::string& securityIdentifier);
};

class InvalidHoldingLimitsException : public ExpectedException {
public:
	InvalidHoldingLimitsException(const std::string& message);
};

class IOException : public ExpectedException {
public:
	IOException();
};

class InputFileException : public ExpectedException {
public:
	InputFileException(const std::string& inputFileName);
};

class OutputFileException : public ExpectedException {
public:
	OutputFileException(const std::string& outputFileName);
};

class MissingArgumentException : public ExpectedException {
public:
	MissingArgumentException(std::string message);
};

class RepeatedSpecificationOfVariableException : public ExpectedException {
public:
	RepeatedSpecificationOfVariableException(std::string variableDescription);
};

class SolverInitialisationException : public ExpectedException {
public:
	SolverInitialisationException();
};

class InsufficientMemoryException : public ExpectedException {
public:
	InsufficientMemoryException();
};

class OptimisationInterruptedException : public ExpectedException {
public:
	OptimisationInterruptedException();
};

class RequiredColumnNotFoundException : public ExpectedException {
public:
	RequiredColumnNotFoundException(std::string columnName, std::string fileName);
};

class IdentifierNotRecognisedException : public ExpectedException {
public:
	IdentifierNotRecognisedException(std::string identifier);
};

class InvalidLimitSumException : public ExpectedException {
public:
	InvalidLimitSumException();
};

class InvalidLimitsException : public ExpectedException {
public:
	InvalidLimitsException(const std::string& message);
};

class InvalidCommandException : public ExpectedException {
public:
	InvalidCommandException(const std::string& inputFileName);
};

class ExcessiveSizeException : public ExpectedException {
public:
	ExcessiveSizeException(const std::string& message);
};

class IncompatibleInputArgumentsException : public ExpectedException {
public:
	IncompatibleInputArgumentsException(const std::string& message);
};

class UnexpectedException : public ExpectedException {
public:
	UnexpectedException();
};

class CSVStreamException : public ExpectedException {
public:
	CSVStreamException(const csvstream_exception& exception);
};

#endif // #ifndef DISPERSE_EXCEPTIONS
