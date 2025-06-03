
#include "ProportionLimitedObject.hpp"
#include "Exceptions.hpp"

ProportionLimitedObject::ProportionLimitedObject()
    : maxProportion(1), minProportion(0) {
}

ProportionLimitedObject::ProportionLimitedObject(const double maxProportion, const double minProportion)
    : maxProportion(maxProportion), minProportion(minProportion) {
    verifyLimits();
}

void ProportionLimitedObject::verifyLimits() {
    if (maxProportion > 1) {
        throw InvalidLimitsException("The maximum proportion,  " + std::to_string(maxProportion) + " is greater than 100%.");
    }
    if (minProportion < 0) {
        throw InvalidLimitsException("The minimum proportion, " + std::to_string(minProportion) + ", is negative.");
    }
    if (maxProportion < minProportion) {
        throw InvalidLimitsException(
            "The maximum, " + std::to_string(maxProportion) + ", is less than minimum, " + std::to_string(minProportion)
        );
    }
}

void ProportionLimitedObject::setMaxProportion(const double maxProportion) {
    this->maxProportion = maxProportion;
    verifyLimits();
}

double ProportionLimitedObject::getMaxProportion() const {
    return maxProportion;
}

void ProportionLimitedObject::setMinProportion(const double minProportion) {

    this->minProportion = minProportion;
    verifyLimits();
}

double ProportionLimitedObject::getMinProportion() const {
    return minProportion;
}

bool ProportionLimitedObject::hasConstrainedProportion() const {
    return getMinProportion() > 0.0 || getMaxProportion() < 1.0;
}
