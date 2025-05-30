
#ifndef CSV_OUTPUT
#define CSV_OUTPUT

#include "ExpectedException.hpp"

#pragma warning(push, 0)
#include <string>
#include <fstream>
#pragma warning(pop)

class AbstractCSVOutput {
public:
	virtual void writeElement(double number) = 0;
	virtual void writeElement(std::string text) = 0;
	virtual void finishLine() = 0;
};

class CSVOutput : public AbstractCSVOutput
{
public:
	CSVOutput(const std::string& fileName);
	virtual void writeElement(double number);
	virtual void writeElement(std::string text);
	virtual void finishLine();

private:
	const std::string m_fileName;
	std::ofstream m_outputStream;
	bool firstOnLine;

	void insertCommaIfNecessary();
};

#endif // CSV_OUTPUT
