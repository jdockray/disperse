
#ifndef DISPERSE_SECURITY
#define DISPERSE_SECURITY

#include "QuickMapping.hpp"
#pragma warning(push, 0)
#include <optional>
#pragma warning(pop)

class Security : public IdentifiedObject, public ObjectWithMaxAndMinProportions
{
public:
	Security(const std::string& identifier);
	
	void setExpectedReturn(const double expectedReturn);
	double getExpectedReturn() const;
	void setRisk(const double risk);
	double getRisk() const;
	void setGroup(const std::string group);
	std::string getGroup() const;

	void addExposure(std::string factorName, double exposure);
	const std::map<std::string, double>& getExposures() const;

private:
	inline static const std::string RESIDUAL_FACTOR_NAME_PREFIX = "";
	inline static const std::string RESIDUAL_FACTOR_NAME_POSTFIX = " residual";
	const std::string residualFactorName;

	double expectedReturn;
	double risk;
	std::string group;
	std::map<std::string, double> exposures;
	double remainingExposure; // To avoid repeated lookup
};

class ListOfSecurities : public IdentifiedObjectList<Security>, public ListOfObjectsWithMaxAndMinProportions<Security>
{
public:
	const std::vector<Security>& getObjects() const;
	std::vector<std::string> getAllFactors() const;
	std::set<std::string> getAllGroups() const;
};

#endif // #ifndef DISPERSE_SECURITY
