
#include "CSVOutput.hpp"

CSVOutput::CSVOutput(std::string fileName)
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

void CSVOutput::writeElement(double number)
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

void CSVOutput::writeElement(std::string text)
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

void CSVOutput::finishLine()
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

void CSVOutput::insertCommaIfNecessary()
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
