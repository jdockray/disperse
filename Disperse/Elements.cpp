
#include "Elements.hpp"
#include "Input.hpp"
#include "Output.hpp"
#include "Exceptions.hpp"

#pragma warning(push, 0)
#include <map>
#pragma warning(pop)

Element::Element(const std::string& row, const std::string& column)
    : row(row), column(column) {
}

std::string Element::getColumn() const {
    return column;
}

std::string Element::getRow() const {
    return row;
}

bool Element::operator==(const Element& other) const {
    return row == other.row && column == other.column;
}

bool Element::operator!=(const Element& other) const {
    return !operator==(other);
}

bool Element::operator<(const Element& other) const {
    if (row < other.row) {
        return true;
    }
    else if (row == other.row) {
        return column < other.column;
    }
    else {
        return false;
    }
}

bool Element::operator<=(const Element& other) const {
    return operator<(other) || operator==(other);
}

bool Element::operator>=(const Element& other) const {
    return !operator<(other);
}

bool Element::operator>(const Element& other) const {
    return !operator<=(other);
}

SparseMatrix elementMatrixFromVector(const std::vector<std::pair<Element, double>>& elements,
    std::vector<std::string>& placeForRowHeadings, std::vector<std::string>& placeForColumnHeadings) {
    std::map<std::string, std::size_t> rowLookup;
    std::map<std::string, std::size_t> columnLookup;
    for (const std::pair<Element, double>& elementPair : elements) {
        std::map<std::string, std::size_t>::const_iterator rowHeading = rowLookup.find(elementPair.first.getRow());
        if (rowHeading == rowLookup.end()) {
            rowLookup[elementPair.first.getRow()] = placeForRowHeadings.size();
            placeForRowHeadings.push_back(elementPair.first.getRow());
        }
        std::map<std::string, std::size_t>::const_iterator columnHeading = columnLookup.find(elementPair.first.getColumn());
        if (columnHeading == columnLookup.end()) {
            columnLookup[elementPair.first.getColumn()] = placeForColumnHeadings.size();
            placeForColumnHeadings.push_back(elementPair.first.getColumn());
        }
    }
    SparseMatrix matrix(placeForRowHeadings.size(), placeForColumnHeadings.size());
    for (const std::pair<Element, double>& elementPair : elements) {
        matrix.setValue(rowLookup[elementPair.first.getRow()], columnLookup[elementPair.first.getColumn()], elementPair.second);
    }
    return matrix;
}
