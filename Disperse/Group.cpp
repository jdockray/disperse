
#include "Group.hpp"
#include "ExpectedException.hpp"

Group::Group(const std::string& identifier)
	: identifier(identifier)
{
}

void Group::verifyMaxGreaterThanMin()
{
	if (getMaxProportion() < getMinProportion())
	{
		throw InvalidGroupLimitsException("The group maximum is less than the group minimum for " + identifier + ".");
	}
}

void Group::setMaxProportion(const double maxProportion)
{
	if (getMaxProportion() > 1)
	{
		throw InvalidGroupLimitsException("The amount which can be placed in " + identifier + " is greater than 100%.");
	}
	this->maxProportion = maxProportion;
	verifyMaxGreaterThanMin();
}

double Group::getMaxProportion() const
{
	return maxProportion;
}

void Group::setMinProportion(const double minProportion)
{
	if (getMinProportion() < 0)
	{
		throw InvalidGroupLimitsException("The amount which can be placed in " + identifier + " is negative.");
	}
	this->minProportion;
	verifyMaxGreaterThanMin();
}

double Group::getMinProportion() const
{
	return minProportion;
}

bool Group::hasConstrainedProportion() const
{
	return getMinProportion() > 0 || getMaxProportion() < 1;
}

void ListOfGroups::addGroup(const Group& group)
{
	m_groupLookup.insert(std::pair<std::string, size_t>(group.identifier, m_groups.size()));
	m_groups.push_back(group);
}

const std::vector<Group>& ListOfGroups::getGroups() const
{
	return m_groups;
}

const Group& ListOfGroups::getGroup(const std::string& groupName) const
{
	try
	{
		return m_groups.at(m_groupLookup.at(groupName));
	}
	catch (std::out_of_range)
	{
		throw SecurityNotRecognisedException(groupName);
	}
}

Group& ListOfGroups::getGroup(const std::string& groupName)
{
	try
	{
		return m_groups.at(m_groupLookup.at(groupName));
	}
	catch (std::out_of_range)
	{
		throw SecurityNotRecognisedException(groupName);
	}
}

const Group& ListOfGroups::getGroup(const unsigned int groupNumber) const
{
	return m_groups.at(groupNumber);
}

Group& ListOfGroups::getGroup(const unsigned int groupNumber)
{
	return m_groups.at(groupNumber);
}

std::vector<std::string> ListOfGroups::getIdentifiers() const
{
	std::vector<std::string> identifiers;
	for (auto security : m_groups)
	{
		identifiers.push_back(security.identifier);
	}
	return identifiers;
}

size_t ListOfGroups::size() const
{
	return m_groups.size();
}

size_t ListOfGroups::numberOfConstrainedGroups() const
{
	size_t count = 0;
	for (auto group : m_groups)
	{
		if (group.hasConstrainedProportion()) ++count;
	}
	return count;
}

void ListOfGroups::verifyProportions() const
{
	double sumOfMaxima = 0;
	double sumOfMinima = 0;
	for (const Group& group : m_groups)
	{
		sumOfMaxima += group.getMaxProportion();
		sumOfMinima += group.getMinProportion();
	}
	if (sumOfMaxima < 1)
	{
		throw InvalidGroupLimitsException("The maximum proportions of each group allowed add up to less than 100%");
	}
	if (sumOfMinima > 1)
	{
		throw InvalidGroupLimitsException("The minimum proportions of each group allowed add up to more than 100%");
	}
}
