
#include "Security.hpp"
#include "ExpectedException.hpp"

const std::string DEFAULT_GROUP_NAME = "(Default)";

Security::Security(const std::string& identifier)
	:	IdentifiedObject(identifier), expectedReturn(1), risk(1), group(DEFAULT_GROUP_NAME),
		residualFactorName(RESIDUAL_FACTOR_NAME_PREFIX + identifier + RESIDUAL_FACTOR_NAME_POSTFIX),
		remainingExposure(1)
{
	exposures[residualFactorName] = remainingExposure;
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
	NegativeRiskException::verify(risk, getIdentifier());
	this->risk = risk;
}

double Security::getRisk() const
{
	return risk;
}

void Security::setGroup(const std::string group)
{
	this->group = group;
}

std::string Security::getGroup() const
{
	return group;
}

void Security::addExposure(std::string factorName, double exposure)
{
	RepeatedSpecificationOfVariableException::verifyNotSet(factorName, exposures,
		"exposure to " + factorName + " for security " + getIdentifier());
	remainingExposure -= exposure;
	if (remainingExposure < 0)
	{
		throw InvalidLimitSumException();
	}
	exposures[factorName] = exposure;
	if (remainingExposure == 0)
	{
		exposures.erase(residualFactorName);
	}
	else
	{
		exposures[residualFactorName] = remainingExposure;
	}
}

const std::map<std::string, double>& Security::getExposures() const
{
	return exposures;
}

const std::vector<Security>& ListOfSecurities::getObjects() const
{
	return IdentifiedObjectList<Security>::getObjects();
}

std::vector<std::string> ListOfSecurities::getAllFactors() const
{
	std::vector<std::string> factors;
	std::set<std::string> included;
	for (const auto security: this->getObjects())
	{
		for (auto exposures : security.getExposures())
		{
			if (included.find(exposures.first) == included.end())
			{
				factors.push_back(exposures.first);
				included.insert(exposures.first);
			}
		}
	}
	return factors;
}

std::set<std::string> ListOfSecurities::getAllGroups() const
{
	std::set<std::string> groups;
	for (const Security security : this->getObjects())
	{
		if (groups.find(security.getGroup()) == groups.end())
		{
			groups.insert(security.getGroup());
		}
	}
	return groups;
}
