
#ifndef DISPERSE_PROPORTION_LIMITED_OBJECT
#define DISPERSE_PROPORTION_LIMITED_OBJECT

#include "Exceptions.hpp"

#pragma warning(push, 0)
#include <cstddef>
#include <vector>
#pragma warning(pop)

class ProportionLimitedObject {
public:
    ProportionLimitedObject();
    ProportionLimitedObject(const double maxProportion, const double minProportion);

    void setMaxProportion(const double maxProportion);
    double getMaxProportion() const;
    void setMinProportion(const double minProportion);
    double getMinProportion() const;
    bool hasConstrainedProportion() const;

private:
    double maxProportion;
    double minProportion;

    void verifyLimits();
};

template <class T>
class ProportionLimitedObjectList {
public:
    std::size_t numberOfConstrainedObjects() const {
        std::size_t count = 0;
        for (const T& object : getObjects()) {
            if (object.hasConstrainedProportion()) ++count;
        }
        return count;
    }

    void verifyProportions() const {
        double sumOfMaxima = 0;
        double sumOfMinima = 0;
        for (const T& object : getObjects()) {
            sumOfMaxima += object.getMaxProportion();
            sumOfMinima += object.getMinProportion();
        }
        if (sumOfMaxima < 1) {
            throw InvalidLimitsException("The maximum proportions add up to less than 100%");
        }
        if (sumOfMinima > 1) {
            throw InvalidLimitsException("The minimum proportions add up to more than 100%");
        }
    }

protected:
    virtual const std::vector<T>& getObjects() const = 0;
};

#endif // #ifndef DISPERSE_PROPORTION_LIMITED_OBJECT
