#pragma once

#include <string>
#include <list>
#include <map>
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
};
