
#ifndef DISPERSE_QUICK_MAP
#define DISPERSE_QUICK_MAP

#pragma warning(push, 0)
#include <string>
#include <vector>
#include <map>
#pragma warning(pop)

#include "ExpectedException.hpp"

class IdentifiedObject
{
public:
	IdentifiedObject(const std::string& identifier);
	std::string getIdentifier() const;

private:
	std::string m_identifier;
};

class ObjectWithMaxAndMinProportions
{
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
class IdentifiedObjectList
{
	static_assert(std::is_base_of<IdentifiedObject, T>::value);

public:
	void add(const T& element)
	{
		m_identifiers.push_back(element.getIdentifier());
		m_objects.push_back(element);
		m_lookup[element.getIdentifier()] = m_objects.size();
	}

	const T& get(const std::string& identifier) const
	{
		try
		{
			return at(m_lookup.at(identifier));
		}
		catch (std::out_of_range error)
		{
			throw IdentifierNotRecognisedException(identifier);
		}
	}

	T& get(const std::string& identifier)
	{
		{
			try
			{
				return at(m_lookup.at(identifier));
			}
			catch (std::out_of_range error)
			{
				throw IdentifierNotRecognisedException(identifier);
			}
		}
	}

	const T& at(const unsigned int index) const
	{
		return m_objects.at(index);
	}

	T& at(const unsigned int index)
	{
		return m_objects.at(index);
	}

	const std::vector<std::string> getIdentifiers() const
	{
		return m_identifiers;
	}

	const std::vector<T>& getObjects() const
	{
		return m_objects;
	}

	size_t size() const
	{
		return m_objects.size();
	}

private:
	std::vector<std::string> m_identifiers;
	std::vector<T> m_objects;
	std::map<std::string, size_t> m_lookup;
};

template <class T>
class ListOfObjectsWithMaxAndMinProportions
{
public:
	size_t numberOfConstrainedGroups() const
	{
		size_t count = 0;
		for (const T& object : getObjects())
		{
			if (object.hasConstrainedProportion()) ++count;
		}
		return count;
	}
	
	void verifyProportions() const
	{
		double sumOfMaxima = 0;
		double sumOfMinima = 0;
		for (const T& object : getObjects())
		{
			sumOfMaxima += object.getMaxProportion();
			sumOfMinima += object.getMinProportion();
		}
		if (sumOfMaxima < 1)
		{
			throw InvalidLimitsException("The maximum proportions add up to less than 100%");
		}
		if (sumOfMinima > 1)
		{
			throw InvalidLimitsException("The minimum proportions add up to more than 100%");
		}
	}

protected:
	virtual const std::vector<T>& getObjects() const = 0;
};

#endif // #ifndef DISPERSE_GROUP
