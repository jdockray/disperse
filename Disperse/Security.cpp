
#include "Security.hpp"
#include "ExpectedException.hpp"

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

bool Security::hasConstrainedProportion() const
{
	return getMinProportion() > 0 || getMaxProportion() < 1;
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

void ListOfSecurities::addSecurity(const Security& security)
{
	m_securityLookup.insert(std::pair<std::string, size_t>(security.identifier, m_securities.size()));
	m_securities.push_back(security);
}

const std::vector<Security>& ListOfSecurities::getSecurities() const
{
	return m_securities;
}

const Security& ListOfSecurities::getSecurity(const std::string& securityName) const
{
	try
	{
		return m_securities.at(m_securityLookup.at(securityName));
	}
	catch (std::out_of_range)
	{
		throw SecurityNotRecognisedException(securityName);
	}
}

Security& ListOfSecurities::getSecurity(const std::string& securityName)
{
	try
	{
		return m_securities.at(m_securityLookup.at(securityName));
	}
	catch (std::out_of_range)
	{
		throw SecurityNotRecognisedException(securityName);
	}
}

const Security& ListOfSecurities::getSecurity(const unsigned int securityNumber) const
{
	return m_securities.at(securityNumber);
}

Security& ListOfSecurities::getSecurity(const unsigned int securityNumber)
{
	return m_securities.at(securityNumber);
}

std::vector<std::string> ListOfSecurities::getIdentifiers() const
{
	std::vector<std::string> identifiers;
	for (auto security : m_securities)
	{
		identifiers.push_back(security.identifier);
	}
	return identifiers;
}

std::vector<std::string> ListOfSecurities::getAllFactors() const
{
	std::vector<std::string> factors;
	std::set<std::string> included;
	for (auto security : m_securities)
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

size_t ListOfSecurities::size() const
{
	return m_securities.size();
}

size_t ListOfSecurities::numberOfConstrainedSecurities() const
{
	size_t count = 0;
	for (auto security : m_securities)
	{
		if (security.hasConstrainedProportion()) ++count;
	}
	return count;
}

void ListOfSecurities::verifyProportions() const
{
	double sumOfMaxima = 0;
	double sumOfMinima = 0;
	for (const Security& security : m_securities)
	{
		sumOfMaxima += security.getMaxProportion();
		sumOfMinima += security.getMinProportion();
	}
	if (sumOfMaxima < 1)
	{
		throw InvalidHoldingLimitsException("The maximum proportions of each security allowed add up to less than 100%");
	}
	if (sumOfMinima > 1)
	{
		throw InvalidHoldingLimitsException("The minimum proportions of each security allowed add up to more than 100%");
	}
}
