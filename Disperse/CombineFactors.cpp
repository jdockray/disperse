
#include "CombineFactors.hpp"
#include "CmdLine.hpp"
#include "Elements.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <map>
#include <sstream>
#include <memory>
#pragma warning(pop)

std::vector<std::string> getDelimitedElements(const std::string& delimitedList)
{
	std::stringstream listStream;
	listStream << delimitedList;
	std::string tempString;
	std::vector<std::string> list;
	if (std::getline(listStream, tempString, ':'))
	{
		list.push_back(tempString);
	}
	return list;
}

void addElements(const std::vector<std::pair<Element, double>>& newElements, std::map<Element, double>& elementsToAddTo)
{
	for (std::pair<Element, double> element : newElements)
	{
		elementsToAddTo[element.first] += element.second;
	}
}

void runCombineCommand(const std::vector<std::string>& args)
{
	CmdLineArgs cmdLineArgs(args);
	std::map<Element, double> elements;
	const std::optional<std::string> gridInputFileNameString = cmdLineArgs.getSingleArgumentOption('g');
	if (gridInputFileNameString)
	{
		for (const std::string& fileName : getDelimitedElements(gridInputFileNameString.value()))
		{
			addElements(getElementsFromGridFile(fileName), elements);
		}
	}
	const std::optional<std::string> listInputFileNameString = cmdLineArgs.getSingleArgumentOption('l');
	if (listInputFileNameString)
	{
		for (const std::string& fileName : getDelimitedElements(listInputFileNameString.value()))
		{
			addElements(getElementsFromListFile(fileName), elements);
		}
	}
	std::vector<std::pair<Element, double>> pairs;
	for (std::pair<Element, double> element : elements)
	{
		pairs.push_back(element);
	}
	const std::optional<std::string> gridOutputFileName = cmdLineArgs.getSingleArgumentOption('r');
	if (gridOutputFileName)
	{
		putElementsInGridFile(gridOutputFileName.value(), pairs);
	}
	const std::optional<std::string> listOutputFileName = cmdLineArgs.getSingleArgumentOption('i');
	if (listOutputFileName)
	{
		putElementsInListFile(listOutputFileName.value(), pairs);
	}
}

void runMultiplyCommand(const std::vector<std::string>& args)
{
	CmdLineArgs cmdLineArgs(args);
	std::vector<std::vector<std::pair<Element, double>>> elementSets;
	const std::optional<std::string> gridInputFileNameString = cmdLineArgs.getSingleArgumentOption('g');
	if (gridInputFileNameString)
	{
		for (const std::string& fileName : getDelimitedElements(gridInputFileNameString.value()))
		{
			elementSets.push_back(getElementsFromGridFile(fileName));
		}
	}
	const std::optional<std::string> listInputFileNameString = cmdLineArgs.getSingleArgumentOption('l');
	if (listInputFileNameString)
	{
		for (const std::string& fileName : getDelimitedElements(listInputFileNameString.value()))
		{
			elementSets.push_back(getElementsFromListFile(fileName));
		}
	}
	std::vector<std::string> columnHeadings;
	std::vector<std::string> rowHeadings;
	std::vector<std::string> joiningAxisHeadings;
	std::unique_ptr<SparseMatrix> outputMatrix;
	switch (elementSets.size())
	{
	case 1:
		outputMatrix = reset(elementMatrixFromVector(elementSets.at(0), rowHeadings, columnHeadings));

	case 2:


	default:
		throw IncompatibleInputArgumentsException("There must be either 1 or 2 input files.");
	}

	// To be continued
	// Matrix code should be modified to return unique pointers to matrices or move assignment and construction
	// operators should be implemented. Use the rule of five.
}

