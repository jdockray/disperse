
#ifndef DISPERSE_SECURITY
#define DISPERSE_SECURITY

#include "IdentifiedObject.hpp"
#include "ProportionLimitedObject.hpp"

#pragma warning(push, 0)
#include <optional>
#pragma warning(pop)

class Security : public IdentifiedObject, public ProportionLimitedObject {
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
	inline static const std::string default_group_name = "(Default)";
	inline static const std::string residual_factor_name_prefix = "";
	inline static const std::string residual_factor_name_postfix = " residual";

	const std::string residualFactorName;
	double expectedReturn;
	double risk;
	std::string group;
	std::map<std::string, double> exposures;
	double remainingExposure; // To avoid repeated lookup
};

class ListOfSecurities : public IdentifiedObjectList<Security>, public ProportionLimitedObjectList<Security> {
public:
	const std::vector<Security>& getObjects() const;
	std::vector<std::string> getAllFactors() const;
	std::set<std::string> getAllGroups() const;
};

#endif // #ifndef DISPERSE_SECURITY
