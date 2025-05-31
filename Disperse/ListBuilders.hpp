
#ifndef DISPERSE_LIST_BUILDERS
#define DISPERSE_LIST_BUILDERS

#include "Input.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

class SecurityListBuilder {
public:
	SecurityListBuilder(IInput& input);
	void loadFactorsFromGrid(IInput& input);
	void loadFactorsFromList(IInput& input);
	const ListOfSecurities& getSecurityList();

private:
	inline static const std::string security_column_name = "Security";
	inline static const std::string expected_return_column_name = "Expected";
	inline static const std::string risk_column_name = "Risk";
	inline static const std::string minimum_allocation_column_name = "Min";
	inline static const std::string maximum_allocation_column_name = "Max";
	inline static const std::string group_column_name = "Group";

	ListOfSecurities securities;

	static ListOfSecurities inputSecurities(IInput& input);
};

ListOfGroups inputGroups(IInput& input);

#endif // #ifndef DISPERSE_LIST_BUILDERS
