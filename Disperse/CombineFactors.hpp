
#ifndef DISPERSE_COMBINE_FACTORS
#define DISPERSE_COMBINE_FACTORS

#include "Input.hpp"
#include "Output.hpp"
#include "Elements.hpp"
#include "ElementWriters.hpp"

#pragma warning(push, 0)
#include <optional>
#include <vector>
#include <string>
#pragma warning(pop)

void combineElements(std::map<Element, double> elements, double additionalMarketRisk, const std::string& marketRiskName,
    std::vector<std::reference_wrapper<IWriter>>& outputWriters);
void multiplyElements(const std::vector<std::vector<std::pair<Element, double>>>& elementSets, double scalarToMultiplyBy,
    std::vector<std::reference_wrapper<IWriter>>& outputWriters);

#endif // #ifndef DISPERSE_COMBINE_FACTORS
