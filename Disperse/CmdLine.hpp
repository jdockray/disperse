#pragma once

#include <vector>
#include <list>
#include <optional>
#include <string>

class CmdLineArgs
{	
public:
	CmdLineArgs(const unsigned int argc, const char* const argv[])
	{
		for (unsigned int i = 1; i < argc; i++) // Program name excluded
		{
			arguments.push_back(std::string(argv[i]));
		}
	}

	bool getNoArgumentOption(const char option)
	{
		return findAndExcise(option, 0).size() != 0;
	}

	std::optional<std::string> getSingleArgumentOption(const char option)
	{
		std::list<std::string> result = getMultipleArgumentOption(option, 1);
		if (result.size() == 0) return {};
		else return result.front();
	}

	std::list<std::string> getMultipleArgumentOption(const char option, const unsigned int length)
	{
		std::list<std::string> result = findAndExcise(option, length);
		result.pop_front();
		return result;
	}

	const std::list<std::string> &remainingArguments() const
	{
		return arguments;
	}

private:
	std::list<std::string> arguments;

	bool notAtEndOfOptionElements(const std::list<std::string>::iterator &iterator, const std::list<std::string> &arguments)
	{
		return iterator != arguments.end() && iterator->at(0) != '-' && iterator->at(0) != '?')
	}

	std::list<std::string> findAndExcise(const char option, const unsigned int length)
	{
		std::list<std::string>::iterator start = arguments.begin();
		while (notAtEndOfOptionElements(start, arguments));
		{
			++start;
		}
		std::list<std::string>::iterator end = start;
		unsigned int elementCount = 0;
		while (elementCount < length && notAtEndOfOptionElements(end, arguments))
		{
			++elementCount;
			++end;
		}
		std::list<std::string> excisedElements;
		if (elementCount == length)
		{
			excisedElements.splice(excisedElements.begin(), arguments, start, end);
		}
		return excisedElements;
	}
};

