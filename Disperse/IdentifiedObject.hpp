
#ifndef DISPERSE_IDENTIFIED_OBJECT
#define DISPERSE_IDENTIFIED_OBJECT

#pragma warning(push, 0)
#include <string>
#include <vector>
#include <map>
#pragma warning(pop)

#include "Exceptions.hpp"

class IdentifiedObject {
public:
    IdentifiedObject(const std::string& identifier)
        : identifier(identifier) {
    }

    std::string getIdentifier() const {
        return identifier;
    }

private:
    std::string identifier;
};

template <class T>
class IdentifiedObjectList {
    static_assert(std::is_base_of<IdentifiedObject, T>::value);

public:
    void add(const T& element) {
        identifiers.push_back(element.getIdentifier());
        lookup[element.getIdentifier()] = objects.size();
        objects.push_back(element);
    }

    const T& get(const std::string& identifier) const {
        try {
            return at(lookup.at(identifier));
        }
        catch (const std::out_of_range&) {
            throw IdentifierNotRecognisedException(identifier);
        }
    }

    T& get(const std::string& identifier) {
        {
            try {
                return at(lookup.at(identifier));
            }
            catch (const std::out_of_range&) {
                throw IdentifierNotRecognisedException(identifier);
            }
        }
    }

    const T& at(const std::size_t index) const {
        return objects.at(index);
    }

    T& at(const std::size_t index) {
        return objects.at(index);
    }

    const std::vector<std::string> getIdentifiers() const {
        return identifiers;
    }

    const std::vector<T>& getObjects() const {
        return objects;
    }

    std::size_t size() const {
        return objects.size();
    }

private:
    std::vector<std::string> identifiers;
    std::vector<T> objects;
    std::map<std::string, std::size_t> lookup;
};

#endif // #ifndef DISPERSE_IDENTIFIED_OBJECT
