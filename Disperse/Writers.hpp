
#ifndef DISPERSE_WRITERS
#define DISPERSE_WRITERS

#include "Writers.hpp"
#include "Security.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

class IWriter {
public:
    virtual void write(const std::vector<std::string>& keys, const std::vector<double>& values) = 0;
    virtual void write(const std::vector<std::string>& keys, const std::map<std::string, double>& values) = 0;
};

class AllocationWriter : public IWriter {
public:
    AllocationWriter(IOutput& output, const std::string& nameHeading, const std::string& allocationHeading);
    virtual void write(const std::vector<std::string>& keys, const std::vector<double>& values);
    virtual void write(const std::vector<std::string>& keys, const std::map<std::string, double>& values);

private:
    IOutput& output;
};

#endif // #ifndef DISPERSE_WRITERS
