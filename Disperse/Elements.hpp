
#ifndef DISPERSE_ELEMENTS
#define DISPERSE_ELEMENTS

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

private:
	const std::string row;
	const std::string column;
};

std::vector<std::pair<Element, double>> getElementsFromGridFile(const std::string& inputFileName);
std::vector<std::pair<Element, double>> getElementsFromListFile(const std::string& inputFileName);

void putElementsInGridFile(const std::string& outputFileName, const std::vector<std::string>& rows,
							const std::vector<std::string>& columns, const SparseMatrix& matrix);
void putElementsInGridFile(const std::string& outputFileName, const std::vector<std::pair<Element, double>>& elements);

void putElementsInListFile(const std::string& outputFileName, const std::vector<std::string>& rows,
	const std::vector<std::string>& columns, const SparseMatrix& matrix);
void putElementsInListFile(const std::string& outputFileName, const std::vector<std::pair<Element, double>>& elements);

SparseMatrix elementMatrixFromVector(const std::vector<std::pair<Element, double>>& elements,
	std::vector<std::string>& placeForRowHeadings, std::vector<std::string>& placeForColumnHeadings);

#endif // DISPERSE_ELEMENTS
