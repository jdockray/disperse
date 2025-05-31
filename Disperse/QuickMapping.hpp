
#ifndef DISPERSE_QUICK_MAPPING
#define DISPERSE_QUICK_MAPPING

#pragma warning(push, 0)
#include <string>
#include <vector>
#include <map>
#pragma warning(pop)

#include "Exceptions.hpp"

class IdentifiedObject {
public:
	IdentifiedObject(const std::string& identifier);
	std::string getIdentifier() const;

private:
	std::string m_identifier;
};

class ObjectWithMaxAndMinProportions {
public:
	ObjectWithMaxAndMinProportions();
	ObjectWithMaxAndMinProportions(const double maxProportion, const double minProportion);

	void setMaxProportion(const double maxProportion);
	double getMaxProportion() const;
	void setMinProportion(const double minProportion);
	double getMinProportion() const;
	bool hasConstrainedProportion() const;

private:
	double m_maxProportion;
	double m_minProportion;

	void verifyMaxGreaterThanMin();
};

template <class T>
class IdentifiedObjectList {
	static_assert(std::is_base_of<IdentifiedObject, T>::value);

public:
	void add(const T& element) {
		m_identifiers.push_back(element.getIdentifier());
		m_lookup[element.getIdentifier()] = m_objects.size();
		m_objects.push_back(element);
	}

	const T& get(const std::string& identifier) const {
		try {
			return at(m_lookup.at(identifier));
		}
		catch (const std::out_of_range&) {
			throw IdentifierNotRecognisedException(identifier);
		}
	}

	T& get(const std::string& identifier) {
		{
			try {
				return at(m_lookup.at(identifier));
			}
			catch (const std::out_of_range&) {
				throw IdentifierNotRecognisedException(identifier);
			}
		}
	}

	const T& at(const std::size_t index) const {
		return m_objects.at(index);
	}

	T& at(const std::size_t index) {
		return m_objects.at(index);
	}

	const std::vector<std::string> getIdentifiers() const {
		return m_identifiers;
	}

	const std::vector<T>& getObjects() const {
		return m_objects;
	}

	std::size_t size() const {
		return m_objects.size();
	}

private:
	std::vector<std::string> m_identifiers;
	std::vector<T> m_objects;
	std::map<std::string, std::size_t> m_lookup;
};

template <class T>
class ListOfObjectsWithMaxAndMinProportions {
public:
	std::size_t numberOfConstrainedGroups() const {
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

#endif // #ifndef DISPERSE_QUICK_MAPPING
