#include "CmdLine.hpp"

CmdLineArgs::CmdLineArgs(const unsigned int argc, const char* const argv[])
{
	for (unsigned int i = 1; i < argc; i++) // Program name excluded
	{
		arguments.push_back(std::string(argv[i]));
	}
}

bool CmdLineArgs::getNoArgumentOption(const char option)
{
	return findAndExcise(option, 0).size() != 0;
}

std::optional<std::string> CmdLineArgs::getSingleArgumentOption(const char option)
{
	std::list<std::string> result = getMultipleArgumentOption(option, 1);
	if (result.size() == 0) return {};
	else return result.front();
}

std::list<std::string> CmdLineArgs::getMultipleArgumentOption(const char option, const unsigned int length)
{
	std::list<std::string> result = findAndExcise(option, length);
	if (result.size() > 0)
	{
		result.pop_front();
	}
	return result;
}

const std::list<std::string>& CmdLineArgs::remainingArguments() const
{
	return arguments;
}

std::list<std::string> CmdLineArgs::findAndExcise(const char option, const unsigned int length)
{
	std::list<std::string> excisedElements;
	std::list<std::string>::iterator start = arguments.begin();
	const char optionCString[]{ '-', option, '\0' };
	while (start != arguments.end() && *start != optionCString)
	{
		++start;
	}
	if (start == arguments.end())
	{
		return excisedElements;
	}
	std::list<std::string>::iterator end = start;
	int elementCount = -1;
	do
	{
		++elementCount;
		++end;
	} while (elementCount < length && end != arguments.end() && end->at(0) != '-');
	if (elementCount == length)
	{
		excisedElements.splice(excisedElements.begin(), arguments, start, end);
	}
	return excisedElements;
}

