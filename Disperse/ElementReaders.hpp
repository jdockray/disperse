
#ifndef DISPERSE_ELEMENT_READERS
#define DISPERSE_ELEMENT_READERS

#include "Elements.hpp"
#include "Input.hpp"
#include "Output.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class IReader {
public:
	virtual std::vector<std::pair<Element, double>> getElements() = 0;
};

class ListFileReader : public IReader {
public:
	ListFileReader(IInput& input);
	virtual std::vector<std::pair<Element, double>> getElements();

private:
	IInput& input;
};

class GridFileReader : public IReader {
public:
	GridFileReader(IInput& input);
	virtual std::vector<std::pair<Element, double>> getElements();

private:
	IInput& input;
};

#endif // DISPERSE_ELEMENT_READERS
