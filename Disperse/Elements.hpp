
#ifndef DISPERSE_ELEMENTS
#define DISPERSE_ELEMENTS

#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class Element
{
public:
	Element(const std::string& row, const std::string& column);
	std::string getRow() const;
	std::string getColumn() const;

	bool operator==(const Element& other) const;
	bool operator!=(const Element& other) const;
	bool operator<(const Element& other) const;
	bool operator>(const Element& other) const;
	bool operator<=(const Element& other) const;
	bool operator>=(const Element& other) const;

private:
	const std::string row;
	const std::string column;
};

std::vector<std::pair<Element, double>> getElementsFromGridFile(AbstractInput& input);
std::vector<std::pair<Element, double>> getElementsFromListFile(AbstractInput& input);
SparseMatrix elementMatrixFromVector(const std::vector<std::pair<Element, double>>& elements,
	std::vector<std::string>& placeForRowHeadings, std::vector<std::string>& placeForColumnHeadings);

#endif // DISPERSE_ELEMENTS
