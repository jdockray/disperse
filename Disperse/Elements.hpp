
#ifndef DISPERSE_ELEMENTS
#define DISPERSE_ELEMENTS

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class Element
{
public:
	Element(const std::string& row, const std::string& column, double value);
	std::string getRow() const;
	std::string getColumn() const;
	double getValue() const;

private:
	const std::string row;
	const std::string column;
	const double value;
};

std::vector<Element> getElementsFromGridFile(const std::string& inputFileName);

std::vector<Element> getElementsFromListFile(const std::string& inputFileName);

#endif // DISPERSE_ELEMENTS
