
#include "ProportionLimitedObject.hpp"

#include "gtest/gtest.h"

TEST(ProportionLimitedObject, DefaultConstructed) {
	ProportionLimitedObject object;
	EXPECT_EQ(object.getMaxProportion(), 1.0);
	EXPECT_EQ(object.getMinProportion(), 0.0);
	EXPECT_FALSE(object.hasConstrainedProportion());
}

TEST(ProportionLimitedObject, ConstructedWithLimits) {
	static const double min = 0.1;
	static const double max = 0.9;
	ProportionLimitedObject object(max, min);
	EXPECT_TRUE(object.hasConstrainedProportion());
	EXPECT_EQ(object.getMaxProportion(), max);
	EXPECT_EQ(object.getMinProportion(), min);
}

TEST(ProportionLimitedObject, ConstructedWithFixedValue) {
	static const double fixed_value = 0.5;
	ProportionLimitedObject(fixed_value, fixed_value); // Tests whether constructor throws
}

TEST(ProportionLimitedObject, ConstructedWithInvalidLimitsFail) {
	EXPECT_THROW(ProportionLimitedObject(1.1, 0.0), InvalidLimitsException);
	EXPECT_THROW(ProportionLimitedObject(1.0, -0.1), InvalidLimitsException);
	EXPECT_THROW(ProportionLimitedObject(0.0, 1.0), InvalidLimitsException);
}

TEST(ProportionLimitedObject, SetThenGet) {
	static const double min = 0.1;
	static const double max = 0.9;
	ProportionLimitedObject object;
	object.setMaxProportion(max);
	object.setMinProportion(min);
	EXPECT_EQ(object.getMaxProportion(), max);
	EXPECT_EQ(object.getMinProportion(), min);
	EXPECT_TRUE(object.hasConstrainedProportion());
}

TEST(ProportionLimitedObject, InvalidSettingFail) {
	static const double initial_value = 0.5;
	ProportionLimitedObject object(initial_value, initial_value);
	EXPECT_THROW(object.setMaxProportion(1.1), InvalidLimitsException);
	EXPECT_THROW(object.setMaxProportion(0.4), InvalidLimitsException); // Less than minimum
	EXPECT_THROW(object.setMinProportion(-0.1), InvalidLimitsException);
	EXPECT_THROW(object.setMinProportion(0.6), InvalidLimitsException); // Less than maximum (still at initial value)
}

class ProportionTestList : public ProportionLimitedObjectList<ProportionLimitedObject> {
public:
	ProportionTestList(const std::vector<ProportionLimitedObject>& objects)
		: objects(objects) {
	}

protected:
	virtual const std::vector<ProportionLimitedObject>& getObjects() const {
		return objects;
	}

private:
	const std::vector<ProportionLimitedObject>& objects;
};

TEST(ProportionLimitedObjectList, Success) {
	std::vector<ProportionLimitedObject> objects;
	objects.emplace_back(1.0, 0.0); // Unconstrained
	objects.emplace_back(1.0, 0.5); // Constrained
	objects.emplace_back(0.5, 0.0); // Constrained
	ProportionTestList myList(objects);
	EXPECT_EQ(myList.numberOfConstrainedObjects(), 2);
	myList.verifyProportions();
}

TEST(ProportionLimitedObjectList, MinimaSumToOne) {
	std::vector<ProportionLimitedObject> objects;
	objects.emplace_back(1.0, 0.5);
	objects.emplace_back(1.0, 0.5);
	ProportionTestList(objects).verifyProportions();
}

TEST(ProportionLimitedObjectList, MinimaSumToGreaterThanOneFail) {
	std::vector<ProportionLimitedObject> objects;
	objects.emplace_back(1.0, 0.55);
	objects.emplace_back(1.0, 0.55);
	EXPECT_THROW(ProportionTestList(objects).verifyProportions(), InvalidLimitsException);
}

TEST(ProportionLimitedObjectList, MaximaSumToOne) {
	std::vector<ProportionLimitedObject> objects;
	objects.emplace_back(0.5, 0.0);
	objects.emplace_back(0.5, 0.0);
	ProportionTestList(objects).verifyProportions();
}

TEST(ProportionLimitedObjectList, MaximaSumToLessThanOneFail) {
	std::vector<ProportionLimitedObject> objects;
	objects.emplace_back(0.45, 0.0);
	objects.emplace_back(0.45, 0.0);
	EXPECT_THROW(ProportionTestList(objects).verifyProportions(), InvalidLimitsException);
}
