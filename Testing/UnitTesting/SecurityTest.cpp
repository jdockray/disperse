
#include "Security.hpp"

#include "gtest/gtest.h"

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

TEST(ListOfSecurities, GetObjectsHasPublicScope) {
	ListOfSecurities().getObjects();
}

template<typename KEY, typename VALUE>
void addMapKeysToSet(const std::map<KEY, VALUE>& map, std::set<KEY>& set) {
	for (const std::pair<KEY, VALUE>& keyValuePair : map) {
		set.insert(keyValuePair.first);
	}
}

TEST(ListOfSecurities, GetAllFactors) {
	static const std::string factor_x = "Factor X";
	static const std::string factor_y = "Factor Y";
	std::set<std::string> expectedFactorSet;
	Security securityA("Security A");
	securityA.addExposure(factor_x, 0.5);
	addMapKeysToSet(securityA.getExposures(), expectedFactorSet);
	Security securityB("Security B");
	securityB.addExposure(factor_x, 0.3);
	securityB.addExposure(factor_y, 0.3);
	addMapKeysToSet(securityB.getExposures(), expectedFactorSet);
	ListOfSecurities listOfSecurities;
	listOfSecurities.add(securityA);
	listOfSecurities.add(securityB);
	for (const std::string& factor : listOfSecurities.getAllFactors()) {
		EXPECT_EQ(expectedFactorSet.erase(factor), 1); // One element should be removed
	}
	EXPECT_EQ(expectedFactorSet.size(), 0); // No more factors should be expected
}

TEST(ListOfSecurities, GetAllGroups) {
	static const std::string group_name = "Test group";
	ListOfSecurities listOfSecurities;
	EXPECT_EQ(listOfSecurities.getAllGroups().size(), 0);
	Security security("Test security");
	security.setGroup(group_name);
	listOfSecurities.add(security);
	security.setGroup("");
	std::set<std::string> groups = listOfSecurities.getAllGroups();
	ASSERT_EQ(groups.size(), 1);
	EXPECT_EQ(*groups.cbegin(), group_name);
}
