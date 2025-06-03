
#include "Output.hpp"
#include "Writers.hpp"
#include "Security.hpp"
#include "Exceptions.hpp"

AllocationWriter::AllocationWriter(IOutput& output, const std::string& nameHeading, const std::string& allocationHeading)
	: output(output) {
	output.writeElement(nameHeading);
	output.writeElement(allocationHeading);
	output.finishLine();
}

void AllocationWriter::write(
	const std::vector<std::string>& names,
	const std::vector<double>& allocations
) {
	if (names.size() != allocations.size()) {
		throw UnexpectedException();
	}
	for (unsigned int i = 0; i < allocations.size(); ++i) {
		output.writeElement(names.at(i));
		output.writeElement(allocations.at(i));
		output.finishLine();
	}
}

void AllocationWriter::write(
	const std::vector<std::string>& names,
	const std::map<std::string, double>& allocations
) {
	for (unsigned int i = 0; i < allocations.size(); ++i) {
		const std::string name = names.at(i);
		output.writeElement(name);
		std::map<std::string, double>::const_iterator groupEntry = allocations.find(name);
		output.writeElement(groupEntry == allocations.end() ? 0.0 : groupEntry->second);
		output.finishLine();
	}
}


