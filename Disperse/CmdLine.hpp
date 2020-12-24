
#ifndef DISPERSE_COMMAND_LINE
#define DISPERSE_COMMAND_LINE

#include <list>
#include <optional>
#include <string>

class CmdLineArgs
{
public:
	CmdLineArgs(const unsigned int argc, const char* const argv[]);
	bool getNoArgumentOption(const char option);
	std::optional<std::string> getSingleArgumentOption(const char option);
	std::list<std::string> getMultipleArgumentOption(const char option, const unsigned int length);
	const std::list<std::string>& remainingArguments() const;

private:
	std::list<std::string> arguments;

	bool notAtEndOfOptionElements(const std::list<std::string>::iterator& iterator, const std::list<std::string>& arguments);
	std::list<std::string> findAndExcise(const char option, const unsigned int length);
};

#endif // #ifndef DISPERSE_COMMAND_LINE