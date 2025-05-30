
#include "CombineFactors.hpp"
#include "Elements.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <map>
#include <sstream>
#include <memory>
#include <cassert>
#pragma warning(pop)

std::vector<std::string> getDelimitedElements(const std::string& delimitedList)
{
	std::stringstream listStream;
	listStream << delimitedList;
	std::string tempString;
	std::vector<std::string> list;
	while (std::getline(listStream, tempString, ','))
	{
		list.push_back(tempString);
	}
	return list;
}

void addElements(const std::vector<std::pair<Element, double>>& newElements, std::map<Element, double>& elementsToAddTo)
{
	for (const std::pair<Element, double>& element : newElements)
	{
		elementsToAddTo[element.first] += element.second;
	}
}

void runCombineCommand(std::vector<std::reference_wrapper<AbstractInput>>& gridInputs,
	std::vector<std::reference_wrapper<AbstractInput>>& listInputs, double additionalMarketRisk,
	const std::string& marketRiskName, AbstractOutput* gridOutput, AbstractOutput* listOutput)
{
	std::map<Element, double> elements;
	for (AbstractInput& gridInput : gridInputs)
	{
		addElements(getElementsFromGridFile(gridInput), elements);
	}
	for (AbstractInput& listInput : listInputs)
	{
		addElements(getElementsFromListFile(listInput), elements);
	}
	if (additionalMarketRisk != 0) {
		std::map<Element, double> additionalMarketRiskElements;
		for (const std::pair<Element, double>& element : elements)
		{
			additionalMarketRiskElements[Element(element.first.getRow(), marketRiskName)] = additionalMarketRisk;
		}
		elements.insert(additionalMarketRiskElements.begin(), additionalMarketRiskElements.end());
	}
	std::vector<std::pair<Element, double>> pairs;
	for (const std::pair<Element, double>& element : elements)
	{
		pairs.push_back(element);
	}
	if (gridOutput)
	{
		putElementsInGridFile(*gridOutput, pairs);
	}
	if (listOutput)
	{
		putElementsInListFile(*listOutput, pairs);
	}
}

void runMultiplyCommand(std::vector<std::reference_wrapper<AbstractInput>>& gridInputs,
	std::vector<std::reference_wrapper<AbstractInput>>& listInputs, double scalarToMultiplyBy, AbstractOutput* gridOutput,
	AbstractOutput* listOutput)
{
	std::vector<std::vector<std::pair<Element, double>>> elementSets;
	for (AbstractInput& gridInput : gridInputs)
	{
		elementSets.push_back(getElementsFromGridFile(gridInput));
	}
	for (AbstractInput& listInput : listInputs)
	{
		elementSets.push_back(getElementsFromListFile(listInput));
	}
	std::vector<std::string> columnHeadings;
	std::vector<std::string> rowHeadings;
	std::vector<std::string> joiningAxisHeadings1;
	std::vector<std::string> joiningAxisHeadings2;
	std::optional<SparseMatrix> outputMatrix;
	switch (elementSets.size())
	{
	case 1:
		outputMatrix = elementMatrixFromVector(elementSets.at(0), rowHeadings, columnHeadings);
		break;
	case 2:
		outputMatrix = multiply(elementMatrixFromVector(elementSets.at(0), rowHeadings, joiningAxisHeadings1),
			elementMatrixFromVector(elementSets.at(1), joiningAxisHeadings2, columnHeadings));
		assert(joiningAxisHeadings1.size() == joiningAxisHeadings2.size());
		for (std::size_t i = 0; i < joiningAxisHeadings1.size(); ++i)
		{
			if (joiningAxisHeadings1[i] != joiningAxisHeadings2[i])
			{
				throw IncompatibleInputArgumentsException("Row heading " + std::to_string(i) + " (" + joiningAxisHeadings1[i]
					+") of matrix A does not match column heading " + std::to_string(i) + " (" + joiningAxisHeadings2[i]
					+ ") of matrix B.");
			}
		}
		break;
	default:
		throw IncompatibleInputArgumentsException("There must be either 1 or 2 input files.");
	}
	if (scalarToMultiplyBy != 1.0)
	{
		outputMatrix = multiply(outputMatrix.value(), scalarToMultiplyBy);
	}
	if (gridOutput)
	{
		putElementsInGridFile(*gridOutput, rowHeadings, columnHeadings, outputMatrix.value());
	}
	if (listOutput)
	{
		putElementsInListFile(*listOutput, rowHeadings, columnHeadings, outputMatrix.value());
	}
}
