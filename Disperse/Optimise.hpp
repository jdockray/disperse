
#ifndef DISPERSE_OPTIMISE
#define DISPERSE_OPTIMISE

#include "Constraint.hpp"
#include "Input.hpp"
#include "Output.hpp"
#include "Matrices.hpp"
#include "ListBuilders.hpp"
#include "Solver.hpp"

#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)

struct OptimisationResult {
	OptimisationResult(const std::vector<double>& allocations, double resultingRisk, const std::vector<std::string> factorNames,
		const std::vector<double>& resultingFactorExposures, const std::map<std::string, double>& proportionsInEachGroup)
		: allocations(allocations), resultingRisk(resultingRisk), factorNames(factorNames),
		resultingFactorExposures(resultingFactorExposures), proportionsInEachGroup(proportionsInEachGroup) {
	}

	const std::vector<double> allocations;
	const double resultingRisk;
	const std::vector<std::string> factorNames;
	const std::vector<double> resultingFactorExposures;
	const std::map<std::string, double> proportionsInEachGroup;
};

class IOptimisationCode {
public:
	virtual OptimisationResult runOptimisation(ISolver& solver, const ListOfSecurities& securities,
		double minimumReturn, ListOfGroups& groups) = 0;
};

class OptimisationCode : public IOptimisationCode {
public:
	virtual OptimisationResult runOptimisation(ISolver& solver, const ListOfSecurities& securities,
		double minimumReturn, ListOfGroups& groups);

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
};

#endif // DISPERSE_ELEMENTS
