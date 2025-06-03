
#include "Help.hpp"

void printCombineCommandHelp(std::ostream& out) {
    out << std::endl;
    out << "Take matrices of grid or list format (see general help) and combine" << std::endl;
    out << "them to produce a matrix with all elements of the input matrices." << std::endl;
    out << "Where input matrices share elements the values will be added together." << std::endl;
    out << std::endl;
    out << "Usage examples:" << std::endl;
    out << "    Disperse.exe combine -m GridA.csv,GridB.csv -i ListA+B.csv" << std::endl;
    out << "    Disperse.exe combine -l ListA.csv,ListB.csv -r GridA+B.csv" << std::endl;
    out << std::endl;
    out << "Options:" << std::endl;
    out << "    -m Input matrix/matrices as grid CSV file(s), one file name or two separated by a comma" << std::endl;
    out << "    -l Input matrix/matrices as list(s) of non-zero matrix elements, one file name or two separated by a comma" << std::endl;
    out << "    -r Output matrix as grid CSV file" << std::endl;
    out << "    -i Output matrix as list of non-zero matrix elements" << std::endl;
    out << "    -a Add a column with all values set to this value (intended to be for a shared, global, market risk)" << std::endl;
    out << "    -b Give a column created with -a the this heading (rather than the default)" << std::endl;
}

void printHelpCommandHelp(std::ostream& out) {
    out << std::endl;
    out << "Print general help or help for a particular command" << std::endl;
    out << "Usage: Disperse.exe help [command]" << std::endl;
}

void printMultiplyCommandHelp(std::ostream& out) {
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

void printOptimiseCommandHelp(std::ostream& out) {
    out << std::endl;
    out << "Take a list of securities with expected returns, variances and risk factors and a minimum target return" << std::endl;
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
    out << "    Disperse.exe optimise -p OutputFactorWeightings.csv IndependentSecurities.csv OutputPortfolio.csv 7.0" << std::endl;
    out << "    Disperse.exe optimise -m RiskFactors.csv -p OutputFactorWeightings.csv Securities.csv OutputPortfolio.csv 7.0" << std::endl;
    out << "    Disperse.exe optimise -m RiskFactors.csv -g ISAandPensionGroups.csv -p OutputFactorWeightings.csv Securities.csv OutputPortfolio.csv 5.0" << std::endl;
    out << "    Disperse.exe optimise -m RiskFactors.csv -l FundOverlap.csv Securities.csv OutputPortfolio.csv 5.0" << std::endl;
    out << std::endl;
    out << "Optional arguments:" << std::endl;
    out << "    -m Input security full risk matrix as a CSV file(s)" << std::endl;
    out << "    -l Input security sparse risk matrix as a list of non-zero matrix elements" << std::endl;
    out << "    -g Input file name listing groups mentioned in the securities file with their minimum and maximum weights in the generated portfolio" << std::endl;
    out << "    -f Output file name for the weightings of each factor in the optimised portfolio" << std::endl;
    out << "    -p Output file name for the weighting of each group in the optimised portfolio" << std::endl;
    out << std::endl;
    out << "A risk factor matrix indicates the proportion of the variablility in the value of the securities explained by" << std::endl;
    out << "independent risk factors." << std::endl;
    out << "The sum of the risk factors for a security must be less than or equal to 1." << std::endl;
    out << "The risk factor matrix is multiplied by its conjugate transpose to generate a security correlation matrix." << std::endl;
    out << "The total risk of each security (standard deviation of returns) from the security CSV table is then" << std::endl;
    out << "used to turn this correlation matrix into the covariance matrix used in the optimisation." << std::endl;
    out << std::endl;
    out << "See the general help ('Disperse.exe help') for details of full and sparse matrices." << std::endl;
    out << "A full risk factor matrix should have row headings corresponding to the security names and" << std::endl;
    out << "column headings corresponding to independent risk factors." << std::endl;
    out << "A sparse risk factor matrix should have the security in the first column, the risk factor in the second" << std::endl;
    out << "and the correlation in the third. There must be headings 'Row', 'Column' and 'Value' respectively." << std::endl;
    out << "If both -m and -l are supplied, the values of shared elements are added." << std::endl;
    out << std::endl;
    out << "The group input file (if supplied) should be a CSV file with headings:" << std::endl;
    out << "    'Group' - An identifier of the group as used in the security file" << std::endl;
    out << "    'Min'   - The minimum proportion of the portfolio that can be occupied by securities labelled with the group (optional, default 0)" << std::endl;
    out << "    'Max'   - The maximum proportion of the portfolio that can be occupied by securities labelled with the group (optional, default 1)" << std::endl;
}

void printGeneralHelp(std::ostream& out) {
    out << std::endl;
    out << "This is a portfolio optimisation program that accepts as input a list of securities with expected returns" << std::endl;
    out << "and risks and a risk factor matrix and returns the weightings that will deliver a minimum return for the" << std::endl;
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
    out << "    List - A sparse 2D matrix with non-zero elements recorded using three columns with the following headings:" << std::endl;
    out << "           'Row', 'Column' and 'Value', containing the rwo and column headings of the full matrix and cell values" << std::endl;
    out << "" << std::endl;
}

void printHelpForCommand(const std::string& commandForWhichHelpIsRequested, std::ostream& out) {
    if (commandForWhichHelpIsRequested == "combine") {
        printCombineCommandHelp(out);
    }
    else if (commandForWhichHelpIsRequested == "help") {
        printHelpCommandHelp(out);
    }
    else if (commandForWhichHelpIsRequested == "multiply") {
        printMultiplyCommandHelp(out);
    }
    else if (commandForWhichHelpIsRequested == "optimise") {
        printOptimiseCommandHelp(out);
    }
    else {
        printGeneralHelp(out);
    }
}
