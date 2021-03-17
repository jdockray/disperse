
#include "Constraint.hpp"
#include "ExpectedException.hpp"

Constraint::Constraint(const double maximum, const double minimum, const size_t numberOfSecurities)
	: maximum(maximum), minimum(minimum), securityWeights(numberOfSecurities)
{
}

Constraint::Constraint(const double maximum, const double minimum, const size_t numberOfSecurities, const double defaultWeight)
	: maximum(maximum), minimum(minimum), securityWeights(numberOfSecurities)
{
	std::fill(securityWeights.begin(), securityWeights.end(), defaultWeight);
}

Constraint::Constraint(const double maximum, const double minimum, const std::vector<double>& securityMultipliers)
	: maximum(maximum), minimum(minimum), securityWeights(securityMultipliers)
{
}

void Constraint::setMaximum(const double maximum)
{
	this->maximum = maximum;
}

double Constraint::getMaximum() const
{
	return maximum;
}

void Constraint::setMinimum(const double minimum)
{
	this->minimum = minimum;
}

double Constraint::getMinimum() const
{
	return minimum;
}

void Constraint::setWeight(const size_t securityIndex, const double multiplier)
{
	if (securityIndex > securityWeights.size())
	{
		throw UnexpectedException();
	}
	securityWeights[securityIndex] = multiplier;
}

void Constraint::setAllWeights(const double weight)
{
	std::fill(securityWeights.begin(), securityWeights.end(), weight);
}

double Constraint::getWeight(const size_t securityIndex) const
{
	return securityWeights.at(securityIndex);
}
