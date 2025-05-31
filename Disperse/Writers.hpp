
#ifndef DISPERSE_WRITERS
#define DISPERSE_WRITERS

#include "Writers.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

class AllocationOutputWriter {
public:
	AllocationOutputWriter(IOutput& output);
	virtual void outputAllocations(const std::vector<std::string>& factorNames,
		const std::vector<double>& allocations);

private:
	IOutput& output;
};

class FactorExposureWriter {
public:
	FactorExposureWriter(IOutput& output);
	virtual void outputFactorExposures(const std::vector<std::string>& factorNames,
		const std::vector<double>& allocations);

private:
	IOutput& output;
};

class GroupProportionWriter {
public:
	GroupProportionWriter(IOutput& output);

	virtual void outputGroupProportions(const std::vector<std::string>& groupNames,
		const std::map<std::string, double>& groupProportions);

private:
	IOutput& output;
};

#endif // #ifndef DISPERSE_WRITERS
