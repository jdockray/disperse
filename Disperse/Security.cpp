#include "Security.hpp"

Security::Security(const std::string& identifier)
	: identifier(identifier)
{
}

void Security::setExpectedReturn(const double expectedReturn)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(this->expectedReturn, "expected return for security " + identifier);
	this->expectedReturn = expectedReturn;
}

double Security::getExpectedReturn() const
{
	return expectedReturn.value_or(1);
}

void Security::setRisk(const double risk)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(this->risk, "risk for security " + identifier);
	NegativeRiskException::verify(risk, identifier);
	this->risk = risk;
}

double Security::getRisk() const
{
	return risk.value_or(1);
}

void Security::setMaxProportion(const double maxProportion)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(this->maxProportion, "maximum holding for security " + identifier);
	this->maxProportion = maxProportion;
	InvalidHoldingLimitsException::verify(*this);
}

double Security::getMaxProportion() const
{
	return maxProportion.value_or(1);
}

void Security::setMinProportion(const double minProportion)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(this->minProportion, "minimum holding for security " + identifier);
	this->minProportion = minProportion;
	InvalidHoldingLimitsException::verify(*this);
}

double Security::getMinProportion() const
{
	return maxProportion.value_or(1);
}

void Security::addExposure(std::string factorName, double exposure)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(factorName, exposures,
		"exposure to " + factorName + " for security " + identifier);
	exposures[factorName] = exposure;
}

double Security::getExposure(std::string factorName)
{
	std::map<std::string, double>::iterator exposure = exposures.find(factorName);
	return (exposure == exposures.end()) ? 0 : exposure->second;
}
