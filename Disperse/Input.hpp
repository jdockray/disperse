
#ifndef DISPERSE_INPUT
#define DISPERSE_INPUT

#include "CSVInput.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

class SecurityListBuilder {
public:
	SecurityListBuilder(AbstractInput& input);
	void loadFactorsFromGrid(AbstractInput& input);
	void loadFactorsFromList(AbstractInput& input);
	const ListOfSecurities& getSecurityList();

private:
	inline static const std::string security_column_name = "Security";
	inline static const std::string expected_return_column_name = "Expected";
	inline static const std::string risk_column_name = "Risk";
	inline static const std::string minimum_allocation_column_name = "Min";
	inline static const std::string maximum_allocation_column_name = "Max";
	inline static const std::string group_column_name = "Group";

	ListOfSecurities securities;

	static ListOfSecurities inputSecurities(AbstractInput& input);
};

ListOfGroups inputGroups(AbstractInput& input);

#endif // #ifndef DISPERSE_INPUT

