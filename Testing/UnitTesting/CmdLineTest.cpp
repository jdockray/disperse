
#include "CmdLine.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>

TEST(CmdLine, ZeroSize) {
	std::vector<std::string> argv;
	CmdLineArgs args(argv);
	EXPECT_EQ(args.remainingArguments().size(), 0);
	EXPECT_FALSE(args.getNoArgumentOption('a'));
	EXPECT_FALSE(args.getSingleArgumentOption('a').has_value());
	EXPECT_EQ(args.getMultipleArgumentOption('a', 2).has_value(), 0);
}

TEST(CmdLine, OnlyRequiredArgs) {
	std::vector<std::string> argv = { "One", "Two" };
	CmdLineArgs args(argv);
	EXPECT_EQ(args.remainingArguments().size(), 2);
}

TEST(CmdLine, NoArgumentOptions) {
	std::vector<std::string> argv = {"-a", "-b", "-c"};
	CmdLineArgs args(argv);
	EXPECT_TRUE(args.getNoArgumentOption('c'));
	EXPECT_TRUE(args.getNoArgumentOption('b'));
	EXPECT_TRUE(args.getNoArgumentOption('a'));
	EXPECT_EQ(args.remainingArguments().size(), 0);
}

TEST(CmdLine, SingleArgumentOptions) {
	std::vector<std::string> argv = { "-a", "One", "-b", "Two"};
	CmdLineArgs args(argv);
	EXPECT_EQ(args.getSingleArgumentOption('b').value_or(""), "Two");
	EXPECT_EQ(args.getSingleArgumentOption('a').value_or(""), "One");
	EXPECT_EQ(args.remainingArguments().size(), 0);
}

TEST(CmdLine, MultipleArgumentOptions) {
	std::vector<std::string> argv = { "Padding", "-a", "One", "Two", "Padding" };
	CmdLineArgs args(argv);
	std::optional<std::vector<std::string>> optionArguments = args.getMultipleArgumentOption('a', 2);
	ASSERT_TRUE(optionArguments.has_value());
	ASSERT_EQ(optionArguments.value().size(), 2);
	EXPECT_EQ(optionArguments.value().at(0), "One");
	EXPECT_EQ(optionArguments.value().at(1), "Two");
	EXPECT_EQ(args.remainingArguments().size(), 2);
	for (const std::string& remainingArg : args.remainingArguments()) {
		EXPECT_EQ(remainingArg, "Padding");
	}
}
