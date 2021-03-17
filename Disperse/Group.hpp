
#ifndef DISPERSE_GROUP
#define DISPERSE_GROUP

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class Group
{
public:
	Group(const std::string& identifier);

	const std::string identifier;

	void setMaxProportion(const double maxProportion);
	double getMaxProportion() const;
	void setMinProportion(const double minProportion);
	double getMinProportion() const;
	bool hasConstrainedProportion() const;

private:
	double maxProportion;
	double minProportion;

	void verifyMaxGreaterThanMin();
};

class ListOfGroups
{
public:
	void addGroup(const Group& group);
	const std::vector<Group>& getGroups() const;
	const Group& getGroup(const std::string& groupName) const;
	Group& getGroup(const std::string& groupName);
	const Group& getGroup(const unsigned int groupNumber) const;
	Group& getGroup(const unsigned int groupNumber);
	std::vector<std::string> getIdentifiers() const;
	size_t size() const;
	size_t numberOfConstrainedGroups() const;
	void verifyProportions() const;

private:
	std::vector<Group> m_groups;
	std::map<std::string, size_t> m_groupLookup;
};

#endif // #ifndef DISPERSE_GROUP
