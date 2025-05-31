
#include "CSVInput.hpp"
#include "CSVOutput.hpp"
#include "SecurityListBuilder.hpp"
#include "Optimise.hpp"
#include "Output.hpp"
#include "ExpectedException.hpp"
#include "Optimisation.hpp"
#include "Group.hpp"

#pragma warning(push, 0)
#include <iostream>
#include <fstream>
#pragma warning(pop)

template <class T, class U>
std::map<U, std::size_t> generateMappingFromIterable(const T& items)
{
	std::size_t index = 0;
	std::map<U, std::size_t> mapping;
	for (const auto& item : items)
	{
		mapping[item] = index++;
	}
	return mapping;
}

double OptimisationCode::roundToOptimisationTolerance(double value)
{
	return std::round(value * ROUNDING_MULTIPLIER) / ROUNDING_MULTIPLIER;
}

std::vector<double> OptimisationCode::roundToOptimisationTolerance(const std::vector<double>& values)
{
	std::vector<double> roundedValues;
	roundedValues.reserve(values.size());
	for (double value : values)
	{
		roundedValues.push_back(roundToOptimisationTolerance(value));
	}
	return roundedValues;
}

SparseMatrix OptimisationCode::generateFactorMatrix(const ListOfSecurities& securities, const std::vector<std::string> factors)
{
	std::map<std::string, std::size_t> factorMapping = generateMappingFromIterable<std::vector<std::string>, std::string>(factors);
	SparseMatrix factorMatrix(securities.size(), factorMapping.size());
	for (unsigned int i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.at(i);
		for (auto exposure : security.getExposures())
		{
			factorMatrix.setValue(i, factorMapping.at(exposure.first), exposure.second);
		}
	}
	return factorMatrix;
}

std::vector<double> OptimisationCode::getSecurityRisks(const ListOfSecurities& securities)
{
	std::vector<double> risks;
	for (const Security& security : securities.getObjects())
	{
		risks.push_back(security.getRisk());
	}
	return risks;
}

SparseMatrix OptimisationCode::generateUpperTriagonalCorrelationMatrix(const SparseMatrix& factorMatrix)
{
	SparseMatrix sqRootedFactorMatrix(factorMatrix);
	applyToAllNonZeroElements(sqRootedFactorMatrix, [](double(x)) { return sqrt(x); });
	SparseMatrix correlationMatrix = multiply(sqRootedFactorMatrix, getTranspose(sqRootedFactorMatrix));
	SparseMatrix upperTriagonalCorrelationMatrix = upperTriangularMatrix(correlationMatrix);
	for (unsigned int i = 0; i < upperTriagonalCorrelationMatrix.rowCount(); ++i)
	{
		upperTriagonalCorrelationMatrix.setValue(i, i, 1);
	}
	return upperTriagonalCorrelationMatrix;
}

SparseMatrix OptimisationCode::generateUpperTriagonalCovarianceMatrix(const std::vector<double>& securitiesStdDevs, const SparseMatrix& factorMatrix)
{
	SparseMatrix riskDiagonalMatrix = vectorToDiagonalMatrix(securitiesStdDevs);
	return multiply(
		riskDiagonalMatrix,
		generateUpperTriagonalCorrelationMatrix(factorMatrix),
		riskDiagonalMatrix
	);
}

Constraint OptimisationCode::getMinimumReturnConstraint(const double minimumReturn, const ListOfSecurities& securities)
{
	Constraint mimimumReturnConstraint(INFINITY, minimumReturn, securities.size());
	for (std::size_t i = 0; i < securities.size(); ++i)
	{
		mimimumReturnConstraint.setWeight(i, securities.at(i).getExpectedReturn());
	}
	return mimimumReturnConstraint;
}

void OptimisationCode::addSecurityConstraints(const ListOfSecurities& securities, std::vector<Constraint>& constraints)
{
	for (std::size_t i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.at(i);
		constraints.push_back(Constraint(security.getMaxProportion(),
			security.getMinProportion(), securities.size(), i, 1));
	}
}

void OptimisationCode::addGroupConstraints(const ListOfGroups& groups, const ListOfSecurities& securities, std::vector<Constraint>& constraints)
{
	for (Group group : groups.getObjects())
	{
		if (group.hasConstrainedProportion())
		{
			Constraint groupConstraint(group.getMaxProportion(), group.getMinProportion(), securities.size());
			for (std::size_t i = 0; i < securities.size(); ++i)
			{
				const Security& security = securities.at(i);
				if (security.getGroup() == group.getIdentifier())
				{
					groupConstraint.setWeight(i, 1);
				}
			}
			constraints.push_back(groupConstraint);
		}
	}
}

std::vector<Constraint> OptimisationCode::getConstraints(double minimumReturn, const ListOfSecurities& securities, const ListOfGroups& groups)
{
	std::vector<Constraint> constraints;
	constraints.push_back(Constraint(1, 1, securities.size(), 1)); // Weights must add to 1
	constraints.push_back(getMinimumReturnConstraint(minimumReturn, securities));
	addSecurityConstraints(securities, constraints);
	addGroupConstraints(groups, securities, constraints);
	return constraints;
}

std::map<std::string, double> OptimisationCode::getGroupProportions(const ListOfSecurities& securities, const std::vector<double>& solution)
{
	std::map<std::string, double> proportions;
	for (std::size_t i = 0; i < securities.size(); ++i)
	{
		const Security& security = securities.at(i);
		std::map<std::string, double>::iterator groupEntry
			= proportions.find(security.getGroup());
		if (groupEntry == proportions.end())
		{
			proportions[security.getGroup()] = solution.at(i);
		}
		else
		{
			groupEntry->second += solution.at(i);
		}
	}
	return proportions;
}

void OptimisationCode::ensureAllGroupsPresent(ListOfGroups& groups, const std::set<std::string>& requiredGroupNames)
{
	for (const std::string groupName : requiredGroupNames)
	{
		const std::vector<std::string>& currentGroupNames = groups.getIdentifiers();
		if (std::find(currentGroupNames.begin(), currentGroupNames.end(), groupName) == currentGroupNames.end())
		{
			groups.add(groupName);
		}
	}
}

void OptimisationCode::runOptimisation(const ListOfSecurities& securities, IOutput& securityOutput, double minimumReturn,
	IOutput* factorOutput, IInput* groupInput, IOutput* groupOutput)
{
	static const std::string risk_output_string = "Risk: %." + std::to_string(TOLERANCE_DECIMAL_PLACES) + "g\n"; // Round to significant figures

	ListOfGroups groups = groupInput ? inputGroups(*groupInput) : ListOfGroups();
	ensureAllGroupsPresent(groups, securities.getAllGroups());

	const std::vector<std::string> factorNames = securities.getAllFactors();
	const SparseMatrix factorMatrix = generateFactorMatrix(securities, factorNames);
	SparseMatrix covarianceMatrix = generateUpperTriagonalCovarianceMatrix(
		getSecurityRisks(securities), factorMatrix);

	std::vector<double> solution = solve(
		covarianceMatrix,
		getConstraints(minimumReturn, securities, groups)
	);

	printf(risk_output_string.c_str(), roundToOptimisationTolerance(std::sqrt(getSingleValue(
		multiply(vectorToHorizontalMatrix(solution), covarianceMatrix, vectorToVerticalMatrix(solution)))
	)));

	outputAllocations(securities.getIdentifiers(), roundToOptimisationTolerance(solution), securityOutput);
	if (factorOutput)
	{
		outputFactorExposures(
			factorNames,
			roundToOptimisationTolerance(horizontalMatrixToVector(multiply(vectorToHorizontalMatrix(solution), factorMatrix))),
			*factorOutput
		);
	}
	if (groupOutput)
	{
		outputGroupProportions(
			groups.getIdentifiers(),
			getGroupProportions(securities, solution),
			*groupOutput
		);
	}
}

