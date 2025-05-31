
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "Constraint.hpp"
#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "Matrices.hpp"
#include "SecurityListBuilder.hpp"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

class IOptimisationCode {
public:
	virtual void runOptimisation(
		const ListOfSecurities& securities,
		IOutput& securityOutput,
		double minimumReturn,
		IOutput* factorOutput = nullptr,
		IInput* groupInput = nullptr,
		IOutput* groupOutput = nullptr
	) = 0;
};

class OptimisationCode : public IOptimisationCode {
public:
	virtual void runOptimisation(
		const ListOfSecurities& securities,
		IOutput& securityOutput,
		double minimumReturn,
		IOutput* factorOutput = nullptr,
		IInput* groupInput = nullptr,
		IOutput* groupOutput = nullptr
	);

private:
	static void addGroupConstraints(const ListOfGroups& groups, const ListOfSecurities& securities, std::vector<Constraint>& constraints);
	static void addSecurityConstraints(const ListOfSecurities& securities, std::vector<Constraint>& constraints);
	static void ensureAllGroupsPresent(ListOfGroups& groups, const std::set<std::string>& requiredGroupNames);
	static SparseMatrix generateFactorMatrix(const ListOfSecurities& securities, const std::vector<std::string> factors);
	static std::vector<Constraint> getConstraints(double minimumReturn, const ListOfSecurities& securities, const ListOfGroups& groups);
	static std::map<std::string, double> getGroupProportions(const ListOfSecurities& securities, const std::vector<double>& solution);
	static Constraint getMinimumReturnConstraint(const double minimumReturn, const ListOfSecurities& securities);
	static SparseMatrix generateUpperTriagonalCorrelationMatrix(const SparseMatrix& factorMatrix);
	static SparseMatrix generateUpperTriagonalCovarianceMatrix(const std::vector<double>& securitiesStdDevs, const SparseMatrix& factorMatrix);
	static std::vector<double> getSecurityRisks(const ListOfSecurities& securities);
	static double roundToOptimisationTolerance(double value);
	static std::vector<double> roundToOptimisationTolerance(const std::vector<double>& values);
};

#endif // DISPERSE_ELEMENTS
