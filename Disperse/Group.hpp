
#ifndef DISPERSE_GROUP
#define DISPERSE_GROUP

#include "QuickMapping.hpp"

class Group : public IdentifiedObject, public ObjectWithMaxAndMinProportions
{
public:
	Group(const std::string& identifier)
		: IdentifiedObject(identifier)
	{
	}
};

class ListOfGroups : public IdentifiedObjectList<Group>, ListOfObjectsWithMaxAndMinProportions<Group>
{
public:
	const std::vector<Group>& getObjects() const
	{
		return IdentifiedObjectList<Group>::getObjects();
	}
};

#endif // #ifndef DISPERSE_GROUP
