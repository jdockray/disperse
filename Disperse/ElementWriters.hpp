
#ifndef DISPERSE_ELEMENTS_WRITERS
#define DISPERSE_ELEMENTS_WRITERS

#include "Input.hpp"
#include "Output.hpp"
#include "Matrices.hpp"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class IElementWriter {
public:
    virtual void writeElements(const std::vector<std::pair<Element, double>>& elements) = 0;
    virtual void writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix) = 0;
};

class ListFileWriter : public IElementWriter {
public:
    ListFileWriter(IOutput& output);
    virtual void writeElements(const std::vector<std::pair<Element, double>>& elements);
    virtual void writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix);

private:
    IOutput& output;
    std::vector<std::pair<Element, double>> elementVectorFromMatrix(const std::vector<std::string>& rowHeadings,
        const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix);
};

class GridFileWriter : public IElementWriter {
public:
    GridFileWriter(IOutput& output);
    virtual void writeElements(const std::vector<std::pair<Element, double>>& elements);
    virtual void writeElements(const std::vector<std::string>& rowHeadings, const std::vector<std::string>& columnHeadings, const SparseMatrix& matrix);

private:
    IOutput& output;
};

#endif // #ifndef DISPERSE_ELEMENTS_WRITERS
