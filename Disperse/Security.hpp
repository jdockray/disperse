
#ifndef DISPERSE_SECURITY
#define DISPERSE_SECURITY

#include <string>
#include <list>
#include <map>
#include <vector>
#include <optional>
#include "ExpectedException.hpp"

class Security
{
public:
	Security(const std::string& identifier);
	
	const std::string identifier;

	void setExpectedReturn(const double expectedReturn);
	double getExpectedReturn() const;
	void setRisk(const double risk);
	double getRisk() const;
	void setMaxProportion(const double maxProportion);
	double getMaxProportion() const;
	void setMinProportion(const double minProportion);
	double getMinProportion() const;
	void addExposure(std::string factorName, double exposure);
	const std::map<std::string, double>& getExposures() const;

private:
	double expectedReturn;
	double risk;
	double maxProportion;
	double minProportion;
	std::map<std::string, double> exposures;
	double remainingExposure;

	void verifyMaxGreaterThanMin();
};

class ListOfSecurities
{
public:
	void addSecurity(const Security& security)
	{
		m_securityLookup.insert(std::pair<std::string, unsigned int>(security.identifier, m_securities.size()));
		m_securities.push_back(security);		
	}

	const std::vector<Security>& getSecurities() const
	{
		return m_securities;
	}

	const Security& getSecurity(const std::string& securityName) const
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

	Security& getSecurity(const std::string& securityName)
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

	const Security& getSecurity(const unsigned int securityNumber) const
	{
		return m_securities.at(securityNumber);
	}

	Security& getSecurity(const unsigned int securityNumber)
	{
		return m_securities.at(securityNumber);
	}

	std::set<std::string> getAllFactors() const
	{
		std::set<std::string> factors;
		for (auto security : m_securities)
		{
			for (auto exposures : security.getExposures())
			{
				factors.insert(exposures.first);
			}
		}
		return factors;
	}

	unsigned int size() const
	{
		return static_cast<unsigned int>(m_securities.size());
	}

	void verifyProportions() const
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

private:
	std::vector<Security> m_securities;
	std::map<std::string, unsigned int> m_securityLookup;
};

#endif // #ifndef DISPERSE_SECURITY
