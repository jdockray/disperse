# disperse

Disperse is a C++ financial portfolio optimisation program. It is a console application which takes CSV files as input, performs a numerical optimisation and then writes CSV output files. I originally created it for my own use, but later decided to share it online to illustrate my software engineering skills. If I were to criticise the current state, there is insufficient testing and it lacks the comments I would put in code I was developing with others.

## Introduction

One way to put money aside for the long term is to invest it in stocks and shares (equities) or bonds (fixed income investments). In the UK, investments can be held in various wrappers such as an ISA or pension, or unwrapped, but the underlying places to invest are the same. Many providers offer a supermarket in which you can choose where to put your money. While investing in individual companies is possible, it is common to invest in pooled investment funds (mutual funds, ETFs and, essentially, investment trusts) that invest in many companies, often those of an index in the case of a passive fund. This spreads the risk between different companies; it diversifies the investment.

__None of the discussion here constitutes financial advice.__

As an investor, it can be hard to decide how much money to invest in different funds or other vehicles. Funds will usually indicate the proportion of their fund invested in different asset classes (equities, bonds, cash, etc.), in different geographical regions (US, Japan, UK etc.) or in different industries (typically 11 major categories including healthcare, energy and financial services). [See here](https://www.morningstar.co.uk/uk/funds/snapshot/snapshot.aspx?id=F00000SRPN&tab=3) for an example. They will also, by law, give an indication of how much risk the money is exposed to (often indicated by how much the price rises and falls over time, its volatility). While the way to select the best investments might be controversial, even straightforward tasks like trying to place the same amount of money in each of the aforementioned industrial categories is difficult to do without using some software.

Disperse is a program which attempts to determine the asset allocation (proportion in each of a set of specified investments) that has the lowest risk (price volatility) while still achieving a specified minimum expected return. It performs a numerical optimisation using the [OSQP quadratic programming solver](https://osqp.org/) and I would like to draw your attention to the third-party attributions at the end of this readme.

For those familiar with this area, a classic mean-variance portfolio optimisation is performed, but the asset covariance matrix is calculated using a factor model. THe latter means that the exposure of each asset to a set of independent risk factors is entered and this is used to generate the covariance matrix. I thought this would be easier to formulate for input than a covariance matrix, which must have certain properties to be valid.

  I have found Disperse to be useful for achieving a portfolio with certain properties, but the output is highly dependent on and only as valid as the input data and any assumptions made. In practice, using a tool like this still requires a significant amount of time and effort and the correlation and uncertainty of markets means that any gain achieved over manually selecting investments, is probably small.

## Theory

This software accepts as input a list of securities (potential investments), with, for each asset:
- an expected return, $\mu$ e.g. 1.07 (7% return).
- the expected return volatility, $\sigma$, the standard deviation of the value of the security. (See Wikipedia [here](https://en.wikipedia.org/wiki/Modern_portfolio_theory).)
- estimates of the proportion of the variation attributable to each of a set of independent factors (exposures) which are believed to explain the variance. One factor, the market risk, will usually dominate. If these do not sum to 1 for a security, then an additional, independent factor is added to account to explain the remaining variance.

The standard deviations and exposures are used to calculate an estimated asset price covariance matrix.

$\mathbf{\Sigma} = \mathrm{diag}(\mathbf{\sigma})\mathbf{F^\mathrm{T}F}\mathrm{diag}(\mathbf{\sigma})$\
where $\mathbf{\Sigma}$ is the asset covariance matrix,\
$\mathrm{diag}(\mathbf{\sigma})$ is the diagonal matrix of the vector of standard deviations,\
$\mathbf{\sigma} = (\sigma_1, \sigma_2, ..., \sigma_N)$
and $\mathbf{F}$ is a matrix defined by this formula, where $f_{ij}$ is the square root of the relative change in price of security i in response to a change in factor j. For example, if factor j changes by $x$ percent and the price of security i changes by $2x$ percent, then $f_{ij}$ would be $\sqrt{2}$. These values are the square roots of the values in the input exposure matrix described above.

$\mathbf{F^\mathrm{T}F}$ is the correlation matrix suggested by the factor weightings with diagonal elements of 1, and multiplying by the standard deviation diagonal matrix at the start and end produces the covariance matrix, $\mathbf{\Sigma}$, where the diagonal elements correspond to the variances of the assets. Calculating the covariance matrix in this way ensures that it fulfils the mathematical requirements of a valid covariance matrix, specifically, that it is square, symmetrical and [positive semi-definite](https://en.wikipedia.org/wiki/Definite_matrix), as required by the subsequent optimisation.

How the user derives the expected returns and standard deviation of returns is left up to them, but the intention is that they use robust values that reflect the considerable uncertainty associated with the prediction of financial market returns. I have previously used the SSRI (Synthetic Risk Reward Indicator) as a stable indicator of the risk level of mutual funds and assumed [arbitrage pricing theory (APT)](https://en.wikipedia.org/wiki/Arbitrage_pricing_theory) when predicting the return, meaning that the expected return is assumed to correlate with the riskiness of the asset. The factor exposures could be used to estimate the return based on the [Capital Asset Pricing Model](https://en.wikipedia.org/wiki/Capital_asset_pricing_model).

The expected returns and covariance matrix are used to find the optimal portfolio as in classical mean-variance optimisation where the risk is minimised subject to achieving a minimum target return.

From p25 of Fabozzi (2007) [Robust Portfolio Optimization and Management](https://www.amazon.co.uk/Robust-Portfolio-Optimization-Management-Fabozzi/dp/047192122X), Wiley and Sons, Hoboken, New Jersey:

minimise $w^\mathrm{T}\mathbf{\Sigma}w$ (Minimise the price variance)\
subject to\
$w^\mathrm{T}\mathbf{\mu}\geq\mu_0$ (The target return being achieved)\
$\sum_{i}$=1 (All of the portfolio must be allocated)\
$w_i \geq 0$ for all i (No short selling)

The program also allows additional constraints to be placed on the proportion of the portfolio assigned to particular securities or groups of securities. 

A risk-free asset is useful to achieve optimal portfolios at low volatilities (portfolios on the [capital market line](https://en.wikipedia.org/wiki/Efficient_frontier)).

## Usage

This program is run from the command line, specifying input CSV (comma separated variable) files. In addition to facilitating the optimisation, it provides operations useful to generate the factor matrix.

For example usage please see my tests [here](Testing/Integration/README.md).

## Interesting code

I will link to files here so the links break less frequently!

Files relevant to the main code path include:
- [Disperse.cpp](Disperse/Disperse.cpp), which contains the main function.
- [ArgumentUnpacking.cpp](Disperse/ArgumentUnpacking.cpp), which is where the command line arguments are deduced.
- [OptimiseCommand.cpp](Disperse/OptimiseCommand.cpp), where concrete components, each with their own single responsibility (such as reading or writing) are instantiated and injected into the components that depend on them.
- [Optimise.hpp](Disperse/Optimise.hpp), which is the header of the code that interogates the investment-specific objects passed in and formulates the optimisation problem in more general terms involving an objective and constraints.
- [ConstraintUnpacking.cpp](Disperse/ConstraintUnpacking.cpp), which translates the array of optimisation constraints into the vectors and matrix needed for optimisation.
- [Solver.cpp](Disperse/Solver.cpp), which populates the C-style structures required by the solver.
- [SolverSession.cpp](Disperse/SolverSession.cpp), which executes the optimisation, managing error handling and clean-up.

## Third-Party Attribution

### OSQP
Copyright (c) 2019 Bartolomeo Stellato, Goran Banjac, Paul Goulart, Stephen Boyd

This work would not be possible without the [Operator Splitting solver for Quadratic Programs (OSQP)](https://osqp.org/) (Oxford University).

The algorithm is described in this paper:
Stellato, B., Banjac, G., Goulart, P., Bemporad, A. and Boyd, S. (2020) [OSQP: an operator splitting solver for quadratic programs](https://doi.org/10.1007/s12532-020-00179-2), 12(4), 637-672.

For other papers relating to features I did not use, see [here](https://osqp.org/citing/).

OSQP is Licensed under the Apache 2.0 License, which you can read [here](https://www.apache.org/licenses/LICENSE-2.0) and also in the OSQP repository [here](https://github.com/osqp/osqp/blob/c7de4a6748e31be0b91a2cac2eb51625c89ca380/LICENSE). The [OSQP code](https://github.com/osqp/osqp) is on GitHub and I have used it by including it as a submodule within my repository [here](ThirdParty/osqp). By the nature of it being a link to the OSQP repository, the code within the submodule is unmodified; however, I have built it in a non-standard way. Rather than using the configure script and makefiles, I found the code to be simple enough for me to simply include the files in two Visual Studio projects ("osqp" and "amd") and use msbuild to create static libraries. The "osqp" project includes the file [osqp_configure.h](ThirdParty/osqp/osqp_configure.h), modified, and included in my repository [next to the submodule](ThirdParty/osqp).

OSQP includes the external modules AMD and QDLDL (below) along with other software developed at Stanford University and at the University of Oxford according to their [NOTICE file](https://github.com/osqp/osqp/blob/c7de4a6748e31be0b91a2cac2eb51625c89ca380/NOTICE).

### AMD
Copyright (c) 1996-2015, Timothy A. Davis, Patrick R. Amestoy, and Iain S. Duff.

AMD (built by my "amd" Visual Studio project) is a third-party library [included in the OSQP repository](https://github.com/osqp/osqp/tree/c7de4a6748e31be0b91a2cac2eb51625c89ca380/algebra/_common/lin_sys/qdldl/amd). AMD is licensed under the [3-clause BSD License](https://github.com/osqp/osqp/blob/c7de4a6748e31be0b91a2cac2eb51625c89ca380/algebra/_common/lin_sys/qdldl/amd/LICENSE).

### QDLDL
Copyright (c) 1996-2015, Timothy A. Davis, Patrick R. Amestoy, and Iain S. Duff.

In the course of using OSQP, I have also used, from the same [OSQP GitHub organisation](https://github.com/osqp), [QDLDL](https://github.com/osqp/qdldl), "A free LDL factorisation routine for quasi-definite linear systems". Details of QDLDL are included in the OSQP paper above. Like OSQP, QDLDL is also licensed under the [Apache 2.0 License](https://github.com/osqp/qdldl/blob/12dbdf0fe4948c9e012735fd45663bf2661d0ef9/LICENSE). As with OSQP, I have included it as a subrepository, but built it into a static library by simply including all the files in a Visual Studio project, rather than using cmake [as suggested](https://github.com/osqp/qdldl/blob/12dbdf0fe4948c9e012735fd45663bf2661d0ef9/README.md#build).

### dlib
(Various authors)

In order to use OSQP, I have used [dlib](http://dlib.net/) by including [the source code](https://github.com/davisking/dlib), including its license, as a subrepository. "Dlib is a modern C++ toolkit containing machine learning algorithms and tools for creating complex software in C++ to solve real world problems." It is licensed under the [Boost Software License 1.0](https://github.com/davisking/dlib/blob/98d26dfaf5355534df0f82ed5ceebc2034d57cdf/LICENSE.txt).

### csvstream
Copyright (c) Andrew DeOrio <awdeorio@umich.edu>

I have also used [csvstream](https://github.com/awdeorio/csvstream), "an easy-to-use CSV file parser for C++" by [Andrew DeOrio](http://andrewdeorio.com), who works at the University of Michigan. This uses the [MIT License](https://github.com/awdeorio/csvstream/blob/59b85c01d39e46dc1a126430f279379aeb6fca11/LICENSE) and the repository (with its licence) is included in mine as a submodule.

## Licence

This project has no licence. It is example code for the perusal of recruiters and potential employers. You may not use it without my express written permission. See the [LICENSE](LICENSE) file for my warranty disclaimer and limitation of liability.
