
#include "CombineFactorCommands.hpp"
#include "CombineFactors.hpp"
#include "Elements.hpp"
#include "ElementReaders.hpp"
#include "ElementWriters.hpp"

void addElements(const std::vector<std::pair<Element, double>>& newElements, std::map<Element, double>& elementsToAddTo) {
    for (const std::pair<Element, double>& element : newElements) {
        elementsToAddTo[element.first] += element.second;
    }
}

std::vector<std::string> getDelimitedElements(const std::string& delimitedList) {
    std::stringstream listStream;
    listStream << delimitedList;
    std::string tempString;
    std::vector<std::string> list;
    while (std::getline(listStream, tempString, ',')) {
        list.push_back(tempString);
    }
    return list;
}

void runCombineCommand(
    const std::vector<std::string>& gridInputFiles,
    const std::vector<std::string>& listInputFiles,
    double additionalMarketRisk,
    const std::string& marketRiskName,
    const std::optional<std::string>& gridOutputFile,
    const std::optional<std::string>& listOutputFile
) {
    std::map<Element, double> elements;
    for (const std::string& gridInputFile : gridInputFiles) {
        CSVInput input(gridInputFile);
        addElements(GridFileReader(input).getElements(), elements);
    }
    for (const std::string& listInputFile : listInputFiles) {
        CSVInput input(listInputFile);
        addElements(ListFileReader(input).getElements(), elements);
    }
    std::vector<std::reference_wrapper<IWriter>> outputs;
    std::unique_ptr<CSVOutput> gridOutput;
    std::unique_ptr<GridFileWriter> gridFileWriter;
    if (gridOutputFile) {
        gridOutput = std::make_unique<CSVOutput>(gridOutputFile.value());
        gridFileWriter = std::make_unique<GridFileWriter>(*gridOutput);
        outputs.push_back(*gridFileWriter);
    }
    std::unique_ptr<CSVOutput> listOutput;
    std::unique_ptr<ListFileWriter> listFileWriter;
    if (listOutputFile) {
        listOutput = std::make_unique<CSVOutput>(gridOutputFile.value());
        listFileWriter = std::make_unique<ListFileWriter>(*listOutput);
        outputs.push_back(*listFileWriter);
    }
    combineElements(elements, additionalMarketRisk, marketRiskName, outputs);
}

void runMultiplyCommand(
    const std::vector<std::string>& gridInputFiles,
    const std::vector<std::string>& listInputFiles,
    double scalarToMultiplyBy,
    const std::optional<std::string>& gridOutputFile,
    const std::optional<std::string>& listOutputFile
) {
    std::vector<std::vector<std::pair<Element, double>>> elementSets;
    for (const std::string& gridInputFile : gridInputFiles) {
        CSVInput input(gridInputFile);
        elementSets.push_back(GridFileReader(input).getElements());
    }
    for (const std::string& listInputFile : listInputFiles) {
        CSVInput input(listInputFile);
        elementSets.push_back(ListFileReader(input).getElements());
    }
    std::vector<std::reference_wrapper<IWriter>> outputs;
    std::unique_ptr<CSVOutput> gridOutput;
    std::unique_ptr<GridFileWriter> gridFileWriter;
    if (gridOutputFile) {
        gridOutput = std::make_unique<CSVOutput>(gridOutputFile.value());
        gridFileWriter = std::make_unique<GridFileWriter>(*gridOutput);
        outputs.push_back(*gridFileWriter);
    }
    std::unique_ptr<CSVOutput> listOutput;
    std::unique_ptr<ListFileWriter> listFileWriter;
    if (listOutputFile) {
        listOutput = std::make_unique<CSVOutput>(gridOutputFile.value());
        listFileWriter = std::make_unique<ListFileWriter>(*listOutput);
        outputs.push_back(*listFileWriter);
    }
    multiplyElements(elementSets, scalarToMultiplyBy, outputs);
}
