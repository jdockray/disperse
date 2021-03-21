
#include "Output.hpp"
#include "Security.hpp"
#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <fstream>
#pragma warning(pop)

class CSVOutput
{
public:
	CSVOutput(std::string fileName)
		: m_fileName(fileName), firstOnLine(true)
	{
		try
		{
			m_outputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			m_outputStream.open(m_fileName, std::ios_base::out);
		}
		catch (std::ios_base::failure)
		{
			throw OutputFileException(m_fileName);
		}
	}

	void writeElement(double number)
	{
		insertCommaIfNecessary();
		try
		{
			m_outputStream << number;
		}
		catch (std::ios_base::failure)
		{
			throw OutputFileException(m_fileName);
		}
	}

	void writeElement(std::string text)
	{
		insertCommaIfNecessary();
		try
		{
			m_outputStream << "\"" << text << "\"";
		}
		catch (std::ios_base::failure)
		{
			throw OutputFileException(m_fileName);
		}
	}

	void finishLine()
	{
		try
		{
			m_outputStream << std::endl;
		}
		catch (std::ios_base::failure)
		{
			throw OutputFileException(m_fileName);
		}
		firstOnLine = true;
	}

private:
	const std::string m_fileName;
	std::ofstream m_outputStream;
	bool firstOnLine;

	void insertCommaIfNecessary()
	{
		if (!firstOnLine)
		{
			try
			{
				m_outputStream << ",";
			}
			catch (std::ios_base::failure)
			{
				throw OutputFileException(m_fileName);
			}
		}
		firstOnLine = false;
	}
};

void outputAllocations(
	const std::vector<std::string>& securityIdentifiers,
	const std::vector<double>& allocations,
	const std::string& outputFileName
)
{
	if (securityIdentifiers.size() != allocations.size())
	{
		throw UnexpectedException();
	}
	CSVOutput csvOutput(outputFileName);
	csvOutput.writeElement("Asset");
	csvOutput.writeElement("Allocation");
	csvOutput.finishLine();
	for (int i = 0; i < allocations.size(); ++i)
	{
		csvOutput.writeElement(securityIdentifiers.at(i));
		csvOutput.writeElement(allocations.at(i));
		csvOutput.finishLine();
	}
}

void outputFactorExposures(
	const std::vector<std::string>& factorNames,
	const std::vector<double>& allocations,
	const std::string& outputFileName
)
{
	if (factorNames.size() != allocations.size())
	{
		throw UnexpectedException();
	}
	CSVOutput csvOutput(outputFileName);
	csvOutput.writeElement("Factor");
	csvOutput.writeElement("Exposure");
	csvOutput.finishLine();
	for (int i = 0; i < allocations.size(); ++i)
	{
		csvOutput.writeElement(factorNames.at(i));
		csvOutput.writeElement(allocations.at(i));
		csvOutput.finishLine();
	}
}

void outputGroupProportions(
	const std::vector<std::string> groupNames,
	const std::map<std::string, double>& groupProportions,
	const std::string& groupOutputFileName
)
{
	CSVOutput csvOutput(groupOutputFileName);
	csvOutput.writeElement("Group");
	csvOutput.writeElement("Proportion");
	csvOutput.finishLine();
	for (int i = 0; i < groupNames.size(); ++i)
	{
		const std::string groupName = groupNames.at(i);
		csvOutput.writeElement(groupName);
		std::map<std::string, double>::const_iterator groupEntry = groupProportions.find(groupName);
		csvOutput.writeElement(groupEntry == groupProportions.end() ? 0.0 : groupEntry->second);
		csvOutput.finishLine();
	}
}
