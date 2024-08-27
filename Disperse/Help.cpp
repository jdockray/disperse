
#include "Optimisation.hpp"

void printCombineCommandHelp(std::ostream& out) {
	out << std::endl;
	out << "Take matrices of grid or list format (see general help) and combine" << std::endl;
	out << "them to produce a matrix with all elements of the input matrices." << std::endl;
	out << "Where input matrices share elements the values will be added together." << std::endl;
	out << std::endl;
	out << "Usage examples:" << std::endl;
	out << "    Disperse.exe multiply -m GridA.csv,GridB.csv -i ListA+B.csv" << std::endl;
	out << "    Disperse.exe multiply -l ListA.csv,ListB.csv -r GridA+B.csv" << std::endl;
	out << std::endl;
	out << "Options:" << std::endl;
	out << "    -m Input matrix/matrices as grid CSV file(s), one file name or two separated by a comma" << std::endl;
	out << "    -l Input matrix/matrices as list(s) of non-zero matrix elements, one file name or two separated by a comma" << std::endl;
	out << "    -r Output matrix as grid CSV file" << std::endl;
	out << "    -i Output matrix as list of non-zero matrix elements" << std::endl;
}

void printHelpCommandHelp(std::ostream& out) {
	out << std::endl;
	out << "Print general help or help for a particular command" << std::endl;
	out << "Usage: Disperse.exe help [command]" << std::endl;
}

void printMultiplyCommandHelp(std::ostream& out)
{
	out << std::endl;
	out << "Take a matrix of grid or list format (see general help) and optionally, multiply" << std::endl;
	out << "it by another matrix and/or a scalar before outputing it in grid or list format." << std::endl;
	out << std::endl;
	out << "Usage examples:" << std::endl;
	out << "    Disperse.exe multiply -m GridA.csv,GridB.csv -r GridAB.csv" << std::endl;
	out << "    Disperse.exe multiply -l ListA.csv,ListB.csv -i ListAB.csv" << std::endl;
	out << "    Disperse.exe multiply -m Raw.csv -s 0.5 -r Scaled.csv" << std::endl;
	out << "    Disperse.exe multiply -m Grid.csv -i List.csv" << std::endl;
	out << std::endl;
	out << "Options:" << std::endl;
	out << "    -m Input matrix/matrices as grid CSV file(s), one file name or two separated by a comma" << std::endl;
	out << "    -l Input matrix/matrices as list(s) of non-zero matrix elements, one file name or two separated by a comma" << std::endl;
	out << "    -s Multiply by scalar" << std::endl;
	out << "    -r Output matrix as grid CSV file" << std::endl;
	out << "    -i Output matrix as list of non-zero matrix elements" << std::endl;
}

void printOptimiseCommandHelp(std::ostream& out)
{
	out << std::endl;
	out << "Take a list of securities with expected returns, variances and correlations and a minimum target return" << std::endl;
	out << "and generate the portfolio with the minimum predicted variance that will generate the expected return" << std::endl;
	out << std::endl;
	out << "Usage:    Disperse.exe optimise [optional arguments] [securities input file] [securities output file] [minimum expected return]" << std::endl;
	out << "The securities input file should be a CSV containing the securities which can be included in the optimised portfolio." << std::endl;
	out << "It should have columns:" << std::endl;
	out << "    'Security' - An identifier of the security such as the ISIN or name (required)" << std::endl;
	out << "    'Expected' - The expected return (optional, default 0)" << std::endl;
	out << "    'Risk'     - A value proportional to the risk, such as the standard deviation of annual returns (optional, default 0)" << std::endl;
	out << "    'Min'      - The minimum weighting the security can have in the final portfolio (optional, default 0)" << std::endl;
	out << "    'Max'      - The maximum weighting the security can have in the final portfolio (optional, default 1)" << std::endl;
	out << "    'Group'    - An optional group name, allowing minimum and maximum weightings for some securities and thus also portfolio parts" << std::endl;
	out << "The minimum expected return should be a number of the same units as the 'Expected' column of the securities input file." << std::endl;
	out << std::endl;
	out << "Usage examples:" << std::endl;
	out << "    Disperse.exe optimise IndependentSecurities.csv OutputPortfolio.csv 7.0" << std::endl;
	out << "    Disperse.exe optimise -m CorrelationMatrix.csv Securities.csv OutputPortfolio.csv 7.0" << std::endl;
	out << "    Disperse.exe optimise -m CorrelationMatrix.csv -g ISAandPensionGroups.csv Securities.csv OutputPortfolio.csv 5.0" << std::endl;
	out << "    Disperse.exe optimise -m CorrelationMatrix.csv -l FundOverlap.csv Securities.csv OutputPortfolio.csv 5.0" << std::endl;
	out << std::endl;
	out << "Optional arguments:" << std::endl;
	out << "    -m Input security full correlation matrix as a CSV file(s)" << std::endl;
	out << "    -l Input security sparse correlation matrix as a list of non-zero matrix elements" << std::endl;
	out << "    -g Input file name listing groups mentioned in the securities file with their minimum and maximum weights in the generated portfolio" << std::endl;
	out << "    -f Output file name for the weightings of each factor in the optimised portfolio" << std::endl;
	out << "    -p Output file name for the weighting of each group in the optimised portfolio" << std::endl;
	out << "If used, the full correlation matrix CSV file should have column and row headings that are the securitiy names." << std::endl;
	out << "If both - m or -l are supplied, the values of shared elements are added. Correlation matrix values must be between 0 and 1." << std::endl;
	out << "The group input file (if supplied) should be a CSV file with headings:" << std::endl;
	out << "    'Group' - An identifier of the group as used in the security file" << std::endl;
	out << "    'Min'   - The minimum proportion of the portfolio that can be occupied by securities labelled with the group (optional, default 0)" << std::endl;
	out << "    'Max'   - The maximum proportion of the portfolio that can be occupied by securities labelled with the group (optional, default 1)" << std::endl;
}

void printGeneralHelp(std::ostream& out)
{
	out << std::endl;
	out << "This is a portfolio optimisation program that accepts as input a list of securities with expected returns" << std::endl;
	out << "and risks and a correlation matrix and returns the weightings that will deliver a minimum return for the" << std::endl;
	out << "lowest overall risk." << std::endl;
	out << std::endl;
	out << "Usage: Disperse.exe [command] [command specific arguments]" << std::endl;
	out << "it by another matrix and/or a scalar before outputing it in grid or list format." << std::endl;
	out << std::endl;
	out << "Commands:" << std::endl;
	out << "optimise - Perform an optimisation" << std::endl;
	out << "multiply - Multiply matrices or multiply a matrix by a scalar" << std::endl;
	out << "combine  - Combine matrices (full outer join), adding the values of shared matrix elements" << std::endl;
	out << "help     - Display general help or help on a particular command." << std::endl;
	out << "For more information run 'Disperse.exe help [command]'." << std::endl;
	out << "" << std::endl;
	out << "Data is input and output via CSV files." << std::endl;
	out << "There are two supported formats for matrices:" << std::endl;
	out << "    Grid - A full 2D matrix with row and column labels as you might find in a spreadsheet" << std::endl;
	out << "    List - A sparse 2D matrix with non-zero elements recorded using three columns: row label, column label, value" << std::endl;
	out << "" << std::endl;
}

void runHelpCommand(const std::vector<std::string>& args) {
	if (args.size() == 0) {
		printGeneralHelp(std::cout);
		return;
	}
	std::string command = args[0]; // A command for which help is requested
	if (command == "combine")
	{
		printCombineCommandHelp(std::cout);
	}
	else if (command == "help")
	{
		printHelpCommandHelp(std::cout);
	}
	else if (command == "multiply")
	{
		printMultiplyCommandHelp(std::cout);
	}
	else if (command == "optimise")
	{
		printOptimiseCommandHelp(std::cout);
	}
	else {
		printGeneralHelp(std::cout);
	}
}
