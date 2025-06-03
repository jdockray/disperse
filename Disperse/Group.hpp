
#ifndef DISPERSE_GROUP
#define DISPERSE_GROUP

#include "IdentifiedObject.hpp"
#include "ProportionLimitedObject.hpp"

class Group : public IdentifiedObject, public ProportionLimitedObject {
public:
    Group(const std::string& identifier)
        : IdentifiedObject(identifier) {
    }
};

class ListOfGroups : public IdentifiedObjectList<Group>, ProportionLimitedObjectList<Group> {
public:
    const std::vector<Group>& getObjects() const {
        return IdentifiedObjectList<Group>::getObjects();
    }
};

#endif // #ifndef DISPERSE_GROUP
