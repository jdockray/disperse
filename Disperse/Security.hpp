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
	double getExposure(std::string factorName);

private:
	std::optional<double> expectedReturn;
	std::optional<double> risk;
	std::optional<double> maxProportion;
	std::optional<double> minProportion;
	std::map<std::string, double> exposures;
};
