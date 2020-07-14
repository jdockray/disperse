#include "Security.hpp"

Security::Security(const std::string& identifier)
	: identifier(identifier), expectedReturn(1), risk(1), maxProportion(1), minProportion(0)
{
}

void Security::setExpectedReturn(const double expectedReturn)
{
	this->expectedReturn = expectedReturn;
}

double Security::getExpectedReturn() const
{
	return expectedReturn;
}

void Security::setRisk(const double risk)
{
	NegativeRiskException::verify(risk, identifier);
	this->risk = risk;
}

double Security::getRisk() const
{
	return risk;
}

void Security::setMaxProportion(const double maxProportion)
{
	this->maxProportion = maxProportion;
	InvalidHoldingLimitsException::verify(*this);
}

double Security::getMaxProportion() const
{
	return maxProportion;
}

void Security::setMinProportion(const double minProportion)
{
	this->minProportion;
	InvalidHoldingLimitsException::verify(*this);
}

double Security::getMinProportion() const
{
	return maxProportion;
}

void Security::addExposure(std::string factorName, double exposure)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(factorName, exposures,
		"exposure to " + factorName + " for security " + identifier);
	exposures[factorName] = exposure;
}

const std::map<std::string, double>& Security::getExposures() const
{
	return exposures;
}