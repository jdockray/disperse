
#include "QuickMapping.hpp"
#include "Exceptions.hpp"

IdentifiedObject::IdentifiedObject(const std::string& identifier)
	: m_identifier(identifier)
{
}

std::string IdentifiedObject::getIdentifier() const
{
	return m_identifier;
}

ObjectWithMaxAndMinProportions::ObjectWithMaxAndMinProportions()
	: m_maxProportion(1), m_minProportion(0)
{
}

ObjectWithMaxAndMinProportions::ObjectWithMaxAndMinProportions(const double maxProportion, const double minProportion)
	: m_maxProportion(maxProportion), m_minProportion(minProportion)
{
}

void ObjectWithMaxAndMinProportions::verifyMaxGreaterThanMin()
{
	if (getMaxProportion() < getMinProportion())
	{
		throw InvalidLimitsException(
			"The maximum, " + std::to_string(getMaxProportion()) + ", is less than minimum, " + std::to_string(getMinProportion())
		);
	}
}

void ObjectWithMaxAndMinProportions::setMaxProportion(const double maxProportion)
{
	if (getMaxProportion() > 1)
	{
		throw InvalidLimitsException("The maximum proportion,  " + std::to_string(getMaxProportion()) + " is greater than 100%.");
	}
	this->m_maxProportion = maxProportion;
	verifyMaxGreaterThanMin();
}

double ObjectWithMaxAndMinProportions::getMaxProportion() const
{
	return m_maxProportion;
}

void ObjectWithMaxAndMinProportions::setMinProportion(const double minProportion)
{
	if (getMinProportion() < 0)
	{
		throw InvalidLimitsException("The minimum proportion, " + std::to_string(getMinProportion()) + ", is negative.");
	}
	this->m_minProportion;
	verifyMaxGreaterThanMin();
}

double ObjectWithMaxAndMinProportions::getMinProportion() const
{
	return m_minProportion;
}

bool ObjectWithMaxAndMinProportions::hasConstrainedProportion() const
{
	return getMinProportion() > 0.0 || getMaxProportion() < 1.0;
}
