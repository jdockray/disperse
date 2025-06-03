
#include "Constraint.hpp"
#include "Exceptions.hpp"

Constraint::Constraint(const double maximum, const double minimum, const std::size_t numberOfSecurities)
    : maximum(maximum), minimum(minimum), securityWeights(numberOfSecurities) {
}

Constraint::Constraint(const double maximum, const double minimum, const std::size_t numberOfSecurities, const double fillWithWeight)
    : maximum(maximum), minimum(minimum), securityWeights(numberOfSecurities) {
    std::fill(securityWeights.begin(), securityWeights.end(), fillWithWeight);
}

Constraint::Constraint(const double maximum, const double minimum, const std::size_t numberOfSecurities,
    const std::size_t securityWithNonZeroWeight, const double weightOfSecurity)
    : maximum(maximum), minimum(minimum), securityWeights(numberOfSecurities) {
    securityWeights.at(securityWithNonZeroWeight) = weightOfSecurity;
}

Constraint::Constraint(const double maximum, const double minimum, const std::vector<double>& securityMultipliers)
    : maximum(maximum), minimum(minimum), securityWeights(securityMultipliers) {
}

void Constraint::setMaximum(const double maximum) {
    this->maximum = maximum;
}

double Constraint::getMaximum() const {
    return maximum;
}

void Constraint::setMinimum(const double minimum) {
    this->minimum = minimum;
}

double Constraint::getMinimum() const {
    return minimum;
}

void Constraint::setWeight(const std::size_t securityIndex, const double multiplier) {
    if (securityIndex > securityWeights.size()) {
        throw UnexpectedException();
    }
    securityWeights[securityIndex] = multiplier;
}

void Constraint::setAllWeights(const double weight) {
    std::fill(securityWeights.begin(), securityWeights.end(), weight);
}

double Constraint::getWeight(const std::size_t securityIndex) const {
    return securityWeights.at(securityIndex);
}
