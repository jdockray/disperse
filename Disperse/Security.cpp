#include "Security.hpp"

Security::Security(const std::string& identifier)
	:	identifier(identifier), expectedReturn(1),
		risk(1), maxProportion(1), minProportion(0),
		remainingExposure(1)
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

void Security::verifyMaxGreaterThanMin()
{
	if (getMaxProportion() < getMinProportion())
	{
		throw InvalidHoldingLimitsException("The holding maximum is less than the holding minimum for " + identifier + ".");
	}
}

void Security::setMaxProportion(const double maxProportion)
{
	if (getMaxProportion() > 1)
	{
		throw InvalidHoldingLimitsException("The amount which can be placed in " + identifier + " is greater than 100%.");
	}
	this->maxProportion = maxProportion;
	verifyMaxGreaterThanMin();
}

double Security::getMaxProportion() const
{
	return maxProportion;
}

void Security::setMinProportion(const double minProportion)
{
	if (getMinProportion() < 0)
	{
		throw InvalidHoldingLimitsException("The amount which can be placed in " + identifier + " is negative.");
	}	
	this->minProportion;
	verifyMaxGreaterThanMin();
}

double Security::getMinProportion() const
{
	return minProportion;
}

void Security::addExposure(std::string factorName, double exposure)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(factorName, exposures,
		"exposure to " + factorName + " for security " + identifier);
	remainingExposure -= exposure;
	if (remainingExposure < 0)
	{
		throw ExposureSumExceedsOneException();
	}
	exposures[factorName] = exposure;
}

const std::map<std::string, double>& Security::getExposures() const
{
	return exposures;
}
