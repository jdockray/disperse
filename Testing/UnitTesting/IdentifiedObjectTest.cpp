
#include "IdentifiedObject.hpp"

#include "gtest/gtest.h"

TEST(IdentifiedObject, GetIdentifier) {
	static const std::string identifier = "Identifier";
	EXPECT_EQ(IdentifiedObject(identifier).getIdentifier(), identifier);
}

TEST(IdentifiedObjectList, AddGetByIdentifier) {
	static const std::string identifier = "Test object";
	IdentifiedObjectList<IdentifiedObject> myList;
	myList.add(IdentifiedObject(identifier));
	myList.get(identifier); // Calls non-const method
	static_cast<const IdentifiedObjectList<IdentifiedObject>&>(myList).get(identifier); // Calls const method
}

TEST(IdentifiedObjectList, GetNonexistentByIdentifierFail) {
	static const std::string identifier = "Test object";
	IdentifiedObjectList<IdentifiedObject> myList;
	EXPECT_THROW(myList.get(identifier), IdentifierNotRecognisedException); // Calls non-const method
	EXPECT_THROW(static_cast<const IdentifiedObjectList<IdentifiedObject>&>(myList).get(identifier),
		IdentifierNotRecognisedException);  // Calls const method
}

TEST(IdentifiedObjectList, AddGetByIndex) {
	static const std::string identifier = "Test object";
	IdentifiedObjectList<IdentifiedObject> myList;
	myList.add(IdentifiedObject(identifier));
	EXPECT_EQ(myList.size(), 1);
	myList.at(0); // Calls non-const method
	static_cast<const IdentifiedObjectList<IdentifiedObject>&>(myList).at(0); // Calls const method
}

TEST(IdentifiedObjectList, OutOfRangeIndexFail) {
	EXPECT_THROW(IdentifiedObjectList<IdentifiedObject>().at(0), std::exception);
}

TEST(IdentifiedObjectList, GetObjectsEmpty) {
	IdentifiedObjectList<IdentifiedObject> emptyList;
	EXPECT_EQ(emptyList.getIdentifiers().size(), 0);
	EXPECT_EQ(emptyList.getObjects().size(), 0);
}

TEST(IdentifiedObjectList, GetIdentifiers) {
	static const std::string identifier = "Test object";
	IdentifiedObjectList<IdentifiedObject> myList;
	myList.add(IdentifiedObject(identifier));
	std::vector<std::string> identifiers = myList.getIdentifiers();
	ASSERT_EQ(identifiers.size(), 1);
	EXPECT_EQ(identifiers.front(), identifier);
}

TEST(IdentifiedObjectList, GetObjects) {
	IdentifiedObjectList<IdentifiedObject> myList;
	myList.add(IdentifiedObject("Test object"));
	ASSERT_EQ(myList.getObjects().size(), 1);
}
