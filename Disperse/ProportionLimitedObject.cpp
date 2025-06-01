
#include "ProportionLimitedObject.hpp"
#include "Exceptions.hpp"

ProportionLimitedObject::ProportionLimitedObject()
	: m_maxProportion(1), m_minProportion(0) {
}

ProportionLimitedObject::ProportionLimitedObject(const double maxProportion, const double minProportion)
	: m_maxProportion(maxProportion), m_minProportion(minProportion) {
}

void ProportionLimitedObject::verifyMaxGreaterThanMin() {
	if (getMaxProportion() < getMinProportion()) {
		throw InvalidLimitsException(
			"The maximum, " + std::to_string(getMaxProportion()) + ", is less than minimum, " + std::to_string(getMinProportion())
		);
	}
}

void ProportionLimitedObject::setMaxProportion(const double maxProportion) {
	if (getMaxProportion() > 1) {
		throw InvalidLimitsException("The maximum proportion,  " + std::to_string(getMaxProportion()) + " is greater than 100%.");
	}
	this->m_maxProportion = maxProportion;
	verifyMaxGreaterThanMin();
}

double ProportionLimitedObject::getMaxProportion() const {
	return m_maxProportion;
}

void ProportionLimitedObject::setMinProportion(const double minProportion) {
	if (getMinProportion() < 0) {
		throw InvalidLimitsException("The minimum proportion, " + std::to_string(getMinProportion()) + ", is negative.");
	}
	this->m_minProportion;
	verifyMaxGreaterThanMin();
}

double ProportionLimitedObject::getMinProportion() const {
	return m_minProportion;
}

bool ProportionLimitedObject::hasConstrainedProportion() const {
	return getMinProportion() > 0.0 || getMaxProportion() < 1.0;
}
