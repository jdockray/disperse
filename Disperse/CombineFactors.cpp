
#include "CombineFactors.hpp"
#include "Elements.hpp"
#include "ElementWriters.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <map>
#include <sstream>
#include <memory>
#include <cassert>
#pragma warning(pop)

void combineElements(std::map<Element, double> elements, double additionalMarketRisk, const std::string& marketRiskName,
	std::vector<std::reference_wrapper<ElementWriter>>& outputWriters)
{
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
	for (ElementWriter& outputWriter : outputWriters) {
		outputWriter.writeElements(pairs);
	}
}

void multiplyElements(const std::vector<std::vector<std::pair<Element, double>>>& elementSets, double scalarToMultiplyBy,
	std::vector<std::reference_wrapper<ElementWriter>>& outputWriters)
{
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
	for (ElementWriter& outputWriter : outputWriters) {
		outputWriter.writeElements(rowHeadings, columnHeadings, outputMatrix.value());
	}
}
