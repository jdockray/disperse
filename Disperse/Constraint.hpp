
#ifndef DISPERSE_CONSTRAINT
#define DISPERSE_CONSTRAINT

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

class Constraint
{
public:
	Constraint(const double maximum, const double minimum, const size_t numberOfSecurities);
	Constraint(const double maximum, const double minimum, const size_t numberOfSecurities, const double defaultWeight);
	Constraint(const double maximum, const double minimum, const std::vector<double>& securityMultipliers);
	void setMaximum(const double maximum);
	double getMaximum() const;
	void setMinimum(const double minimum);
	double getMinimum() const;
	void setWeight(const size_t securityIndex, const double multiplier);
	void setAllWeights(const double weight);
	double getWeight(const size_t securityIndex) const;

private:
	double maximum;
	double minimum;
	std::vector<double> securityWeights;
};

#endif // #ifndef DISPERSE_CONSTRAINT