
#include "Output.hpp"
#include "Writers.hpp"
#include "Security.hpp"
#include "Exceptions.hpp"

AllocationOutputWriter::AllocationOutputWriter(IOutput& output)
	: output(output)
{
}

void AllocationOutputWriter::outputAllocations(
	const std::vector<std::string>& securityIdentifiers,
	const std::vector<double>& allocations
)
{
	if (securityIdentifiers.size() != allocations.size())
	{
		throw UnexpectedException();
	}
	output.writeElement("Asset");
	output.writeElement("Allocation");
	output.finishLine();
	for (unsigned int i = 0; i < allocations.size(); ++i)
	{
		output.writeElement(securityIdentifiers.at(i));
		output.writeElement(allocations.at(i));
		output.finishLine();
	}
}

FactorExposureWriter::FactorExposureWriter(IOutput& output)
	: output(output)
{
}

void FactorExposureWriter::outputFactorExposures(
	const std::vector<std::string>& factorNames,
	const std::vector<double>& factorExposures
)
{
	if (factorNames.size() != factorExposures.size())
	{
		throw UnexpectedException();
	}
	output.writeElement("Factor");
	output.writeElement("Exposure");
	output.finishLine();
	for (unsigned int i = 0; i < factorExposures.size(); ++i)
	{
		output.writeElement(factorNames.at(i));
		output.writeElement(factorExposures.at(i));
		output.finishLine();
	}
}

GroupProportionWriter::GroupProportionWriter(IOutput& output)
	: output(output)
{
}

void GroupProportionWriter::outputGroupProportions(
	const std::vector<std::string>& groupNames,
	const std::map<std::string, double>& groupProportions
)
{
	output.writeElement("Group");
	output.writeElement("Proportion");
	output.finishLine();
	for (unsigned int i = 0; i < groupNames.size(); ++i)
	{
		const std::string groupName = groupNames.at(i);
		output.writeElement(groupName);
		std::map<std::string, double>::const_iterator groupEntry = groupProportions.find(groupName);
		output.writeElement(groupEntry == groupProportions.end() ? 0.0 : groupEntry->second);
		output.finishLine();
	}
}

