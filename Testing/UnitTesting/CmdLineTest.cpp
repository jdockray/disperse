
#include "CmdLine.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>

/*
class CmdLineArgs {
public:
	CmdLineArgs(const std::vector<std::string>& args);
	bool getNoArgumentOption(const char option);
	std::optional<std::string> getSingleArgumentOption(const char option);
	std::list<std::string> getMultipleArgumentOption(const char option, const unsigned int length);
	const std::list<std::string>& remainingArguments() const;

private:
	std::list<std::string> arguments;

	std::list<std::string> findAndExcise(const char option, const unsigned int length);
};
*/

TEST(CmdLine, ZeroSize) {
	std::vector<std::string> argv;
	CmdLineArgs args(argv);
	EXPECT_EQ(args.remainingArguments().size(), 0);
	EXPECT_FALSE(args.getNoArgumentOption('a'));
	EXPECT_FALSE(args.getSingleArgumentOption('a').has_value());
	EXPECT_EQ(args.getMultipleArgumentOption('a', 2).has_value(), 0);
}
