
#ifndef CSV_OUTPUT
#define CSV_OUTPUT

#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <string>
#include <fstream>
#pragma warning(pop)

class CSVOutput
{
public:
	CSVOutput(std::string fileName);
	void writeElement(double number);
	void writeElement(std::string text);
	void finishLine();

private:
	const std::string m_fileName;
	std::ofstream m_outputStream;
	bool firstOnLine;

	void insertCommaIfNecessary();
};

#endif // CSV_OUTPUT
