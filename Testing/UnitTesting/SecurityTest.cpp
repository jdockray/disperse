
#include "Security.hpp"

#include "gtest/gtest.h"

/*
class ListOfSecurities : public IdentifiedObjectList<Security>, public ProportionLimitedObjectList<Security> {
public:
	const std::vector<Security>& getObjects() const;
	std::vector<std::string> getAllFactors() const;
	std::set<std::string> getAllGroups() const;
};
*/

TEST(Security, SetThenGet) {
	static const double expectedReturn = -0.5;
	static const double risk = 1.5;
	static const std::string group_name = "Test group";
	Security security("");
	security.setExpectedReturn(expectedReturn);
	security.setRisk(risk);
	security.setGroup(group_name);
	EXPECT_EQ(security.getExpectedReturn(), expectedReturn);
	EXPECT_EQ(security.getRisk(), risk);
	EXPECT_EQ(security.getGroup(), group_name);
}

TEST(Security, ExposureAddAndResidualRecalculate) {
	static const std::string factor_a_name = "Factor A";
	static const std::string factor_b_name = "Factor B";
	static const double exposure_to_factor_a = 0.01;
	static const double exposure_to_factor_b = 0.02;
	Security security("");
	ASSERT_EQ(security.getExposures().size(), 1);
	std::string residualName = security.getExposures().cbegin()->first;
	EXPECT_EQ(security.getExposures().cbegin()->second, 1.0);
	security.addExposure(factor_a_name, exposure_to_factor_a);
	security.addExposure(factor_b_name, exposure_to_factor_b);
	ASSERT_EQ(security.getExposures().size(), 3); // Includes residual
	ASSERT_NE(security.getExposures().find(factor_b_name), security.getExposures().cend());
	EXPECT_EQ(security.getExposures().find(factor_b_name)->second, exposure_to_factor_b);
	ASSERT_NE(security.getExposures().find(factor_a_name), security.getExposures().cend());
	EXPECT_EQ(security.getExposures().find(factor_a_name)->second, exposure_to_factor_a);
	EXPECT_EQ(security.getExposures().find(residualName)->second, 1.0 - exposure_to_factor_a - exposure_to_factor_b);
}

TEST(Security, ExposuresExceedOneFail) {
	EXPECT_THROW(Security("").addExposure("", 1.1), InvalidLimitSumException);
}
