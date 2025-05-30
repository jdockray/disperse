
#ifndef CSV_INPUT
#define CSV_INPUT

#pragma warning(push, 0)
#include "csvstream\csvstream\csvstream.hpp"
#pragma warning(pop)

class AbstractInput {
public:
	virtual std::string getName() const = 0;
	virtual std::vector<std::string> getHeader() const = 0;
	virtual std::map<std::string, std::string> readEntryAsMap() = 0;
	virtual std::vector<std::pair<std::string, std::string> > readEntryAsPairVector() = 0;
};

class CSVInput : public AbstractInput {
public:
	CSVInput(const std::string& inputFileName);
	virtual std::string getName() const;
	virtual std::vector<std::string> getHeader() const;
	virtual std::map<std::string, std::string> readEntryAsMap();
	virtual std::vector<std::pair<std::string, std::string> > readEntryAsPairVector();

private:
	const std::string inputFileName;
	csvstream inputStream;

	// Prevent copying
	CSVInput(const CSVInput& other) = delete;
	CSVInput& operator=(const CSVInput& other) = delete;
};

#endif // CSV_INPUT
