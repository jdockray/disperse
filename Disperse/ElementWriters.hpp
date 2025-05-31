
#ifndef ELEMENTS_WRITERS
#define ELEMENTS_WRITERS

#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class IWriter {
public:
	virtual void writeElements(const std::vector<std::pair<Element, double>>& elements) = 0;
	virtual void writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix) = 0;
};

class ListFileWriter : public IWriter
{
public:
	ListFileWriter(IOutput& output);
	virtual void writeElements(const std::vector<std::pair<Element, double>>& elements);
	virtual void writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix);

private:
	IOutput& output;
	std::vector<std::pair<Element, double>> elementVectorFromMatrix(const std::vector<std::string>& rowHeadings,
		const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix);
};

class GridFileWriter : public IWriter
{
public:
	GridFileWriter(IOutput& output);
	virtual void writeElements(const std::vector<std::pair<Element, double>>& elements);
	virtual void writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix);

private:
	IOutput& output;
};

#endif // #ifndef ELEMENTS_WRITERS
