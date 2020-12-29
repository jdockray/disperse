
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
	bool hasConstrainedProportion() const;
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
	void addSecurity(const Security& security);
	const std::vector<Security>& getSecurities() const;
	const Security& getSecurity(const std::string& securityName) const;
	Security& getSecurity(const std::string& securityName);
	const Security& getSecurity(const unsigned int securityNumber) const;
	Security& getSecurity(const unsigned int securityNumber);
	std::vector<std::string> getIdentifiers() const;
	std::vector<std::string> getAllFactors() const;
	unsigned int size() const;
	unsigned int numberOfConstrainedSecurities() const;
	void verifyProportions() const;

private:
	std::vector<Security> m_securities;
	std::map<std::string, unsigned int> m_securityLookup;
};

#endif // #ifndef DISPERSE_SECURITY
