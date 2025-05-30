
#include "CSVOutput.hpp"
#include "Output.hpp"
#include "Security.hpp"
#include "ExpectedException.hpp"

void outputAllocations(
	const std::vector<std::string>& securityIdentifiers,
	const std::vector<double>& allocations,
	AbstractCSVOutput& csvOutput
)
{
	if (securityIdentifiers.size() != allocations.size())
	{
		throw UnexpectedException();
	}
	csvOutput.writeElement("Asset");
	csvOutput.writeElement("Allocation");
	csvOutput.finishLine();
	for (unsigned int i = 0; i < allocations.size(); ++i)
	{
		csvOutput.writeElement(securityIdentifiers.at(i));
		csvOutput.writeElement(allocations.at(i));
		csvOutput.finishLine();
	}
}

void outputFactorExposures(
	const std::vector<std::string>& factorNames,
	const std::vector<double>& allocations,
	AbstractCSVOutput& csvOutput
)
{
	if (factorNames.size() != allocations.size())
	{
		throw UnexpectedException();
	}
	csvOutput.writeElement("Factor");
	csvOutput.writeElement("Exposure");
	csvOutput.finishLine();
	for (unsigned int i = 0; i < allocations.size(); ++i)
	{
		csvOutput.writeElement(factorNames.at(i));
		csvOutput.writeElement(allocations.at(i));
		csvOutput.finishLine();
	}
}

void outputGroupProportions(
	const std::vector<std::string> groupNames,
	const std::map<std::string, double>& groupProportions,
	AbstractCSVOutput& csvOutput
)
{
	csvOutput.writeElement("Group");
	csvOutput.writeElement("Proportion");
	csvOutput.finishLine();
	for (unsigned int i = 0; i < groupNames.size(); ++i)
	{
		const std::string groupName = groupNames.at(i);
		csvOutput.writeElement(groupName);
		std::map<std::string, double>::const_iterator groupEntry = groupProportions.find(groupName);
		csvOutput.writeElement(groupEntry == groupProportions.end() ? 0.0 : groupEntry->second);
		csvOutput.finishLine();
	}
}
