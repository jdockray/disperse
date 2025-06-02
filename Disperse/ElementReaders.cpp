
#include "ElementReaders.hpp"
#include "Parsing.hpp"

void ensureColumnInExpectedPlace(const std::vector<std::string> header, std::size_t index,
	const std::string& columnTitle, const std::string& fileName) {
	if (index >= header.size() || header.at(index) != columnTitle) {
		throw RequiredColumnNotFoundException(columnTitle, fileName);
	}
}

ListFileReader::ListFileReader(IInput& input)
	: input(input) {
}

std::vector<std::pair<Element, double>> ListFileReader::getElements() {
	std::vector<std::string> header = input.getHeader();
	ensureColumnInExpectedPlace(header, 0, "Row", input.getName());
	ensureColumnInExpectedPlace(header, 1, "Column", input.getName());
	ensureColumnInExpectedPlace(header, 2, "Value", input.getName());
	std::vector<std::pair<std::string, std::string> > rowValues = input.readEntryAsPairVector();
	std::vector<std::pair<Element, double>> elements;
	while (!rowValues.empty()) // While vector is not empty
	{
		elements.push_back(std::pair<Element, double>(
			Element(rowValues.at(0).second, rowValues.at(1).second), std::stod(rowValues.at(2).second))
		);
		rowValues = input.readEntryAsPairVector();
	}
	return elements;
}

GridFileReader::GridFileReader(IInput& input)
	: input(input) {
}

std::vector<std::pair<Element, double>> GridFileReader::getElements() {
	std::vector<std::string> header = input.getHeader();
	ensureColumnInExpectedPlace(header, 0, "", input.getName());
	std::vector<std::pair<Element, double>> elements;
	std::string fileContext = "(" + input.getName() + ", ";
	std::vector<std::pair<std::string, std::string> > rowValues = input.readEntryAsPairVector();
	while (!rowValues.empty()) // Checks if vector is empty
	{
		std::vector<std::pair<std::string, std::string>>::const_iterator columnValue = rowValues.begin();
		std::string row = columnValue->second;
		std::string rowContext = fileContext + row + ", ";
		columnValue++;
		while (columnValue != rowValues.end()) {
			elements.push_back(std::pair<Element, double>(
				Element(row, columnValue->first),
				convertToNumber(columnValue->second, rowContext + columnValue->first + ")")
			));
			columnValue++;
		}
		rowValues = input.readEntryAsPairVector();
	}
	return elements;
}
