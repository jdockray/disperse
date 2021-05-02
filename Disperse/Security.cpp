
#include "Security.hpp"
#include "ExpectedException.hpp"

Security::Security(const std::string& identifier)
	:	IdentifiedObject(identifier), expectedReturn(1),
		risk(1), remainingExposure(1)
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

bool Security::hasGroup() const
{
	return group.has_value();
}

std::string Security::getGroup() const
{
	return group.value();
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
}

const std::map<std::string, double>& Security::getExposures() const
{
	return exposures;
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
