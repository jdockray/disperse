
#ifndef DISPERSE_OUTPUT
#define DISPERSE_OUTPUT

#include "CSVOutput.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

/*
class IAllocationOutputWriter {
public:
	virtual void outputFactorExposures(const std::vector<std::string>& factorNames,
		const std::vector<double>& allocations) = 0;
};
*/

class AllocationOutputWriter /* : public IAllocationOutputWriter */ {
public:
	AllocationOutputWriter(IOutput& output);
	virtual void outputAllocations(const std::vector<std::string>& factorNames,
		const std::vector<double>& allocations);

private:
	IOutput& output;
};

/*
class IFactorExposureWriter {
public:
	virtual void outputFactorExposures(const std::vector<std::string>& factorNames,
		const std::vector<double>& allocations) = 0;
};
*/

class FactorExposureWriter /* : public IFactorExposureWriter */ {
public:
	FactorExposureWriter(IOutput& output);
	virtual void outputFactorExposures(const std::vector<std::string>& factorNames,
		const std::vector<double>& allocations);

private:
	IOutput& output;
};

/*
class IGroupProportionWriter {
public:
	virtual void outputGroupProportions(const std::vector<std::string>& groupNames,
		const std::map<std::string, double>& groupProportions) = 0;
};
*/

class GroupProportionWriter /* : public IGroupProportionWriter */ {
public:
	GroupProportionWriter(IOutput& output);

	virtual void outputGroupProportions(const std::vector<std::string>& groupNames,
		const std::map<std::string, double>& groupProportions);

private:
	IOutput& output;
};

#endif // #ifndef DISPERSE_OUTPUT
