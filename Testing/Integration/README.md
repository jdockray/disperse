# Smoke tests

I have added a few integration smoke tests here to demonstrate how this program can be used.
Test.bat contains instructions which can be run on the built executable Disperse.exe to test its functionality.
The required input files are in the 'Input' directory and output files are put in a temporary 'Output' directory, which can be compared with the 'ExpectedOutput' directory here.

## Help messages

The first argument passed to Disperse.exe is always a command. The 'help' command returns useful information in general and about other commands.
You might find reading the output files helpful.

<pre>
    Disperse.exe help ><a href="ExpectedOutput\GeneralHelp.txt">Output\GeneralHelp.txt</a>
    Disperse.exe help combine ><a href="ExpectedOutput\CombineCommandHelp.txt">Output\CombineCommandHelp.txt</a>
    Disperse.exe help multiply ><a href="ExpectedOutput\MultiplyCommandHelp.txt">Output\MultiplyCommandHelp.txt</a>
    Disperse.exe help optimise ><a href="ExpectedOutput\OptimiseCommandHelp.txt">Output\OptimiseCommandHelp.txt</a>
</pre>

## Typical usage

The subsequent commands show data manipulation to generate a risk matrix followed by an optimisation.

The risk matrix (generated using 'multiply' and 'combine' here) indicates the proportion of the variability in each security explained by each of independent risk factors. The optimise command multiplies the risk factor matrix by its conjugate transpose to generate a positive semidefinite security correlation matrix. The total risk associated with each security (the standard deviation of its returns) is used to convert this correlation matrix into a covariance matrix.

In this example the variation of fund returns is modelled to be 85% global market risk, 10% industry-specific risk and 5% within category risk.

<pre>
    Disperse.exe multiply -m <a href="Input\FundRegionWeightings.csv">Input\FundRegionWeightings.csv</a>,<a href="Input\RegionSectorWeightings.csv">Input\RegionSectorWeightings.csv</a> -r <a href="ExpectedOutput\FundSectorWeightings.csv">Output\FundSectorWeightings.csv</a>
    Disperse.exe multiply -m <a href="Input\FundSectorWeightings.csv">Input\FundSectorWeightings.csv</a> -s 0.1 -r <a href="ExpectedOutput\FundSectorEffect.csv">Output\FundSectorEffect.csv</a>
    Disperse.exe multiply -l <a href="Input\ShareOverlap.csv">Input\ShareOverlap.csv</a> -s 0.05 -r <a href="ExpectedOutput\ShareOverlapEffect.csv">Output\ShareOverlapEffect.csv</a>
    Disperse.exe combine -m <a href="Input\FundSectorEffect.csv">Input\FundSectorEffect.csv</a>,<a href="Input\ShareOverlapEffect.csv">Input\ShareOverlapEffect.csv</a> -a 0.85 -r <a href="ExpectedOutput\CombinedExposures.csv">Output\CombinedExposures.csv</a>
    Disperse.exe optimise -m <a href="Input\CombinedExposures.csv">Input\CombinedExposures.csv</a> -f <a href="ExpectedOutput\OutputFactorWeightings.csv">Output\OutputFactorWeightings.csv</a> <a href="Input\EquityFunds.csv">Input\EquityFunds.csv</a> <a href="ExpectedOutput\OutputPortfolio.csv">Output\OutputPortfolio.csv</a> 0.09 ><a href="ExpectedOutput\OptimisationOut.txt">Output\OptimisationOut.txt</a>
</pre>

## Optimise command

Here are some further tests of the optimise command with the required return set to 0 to get the minimum volatility portfolio.

If no risk factors are supplied then the weighting is based solely on the risk (expected standard deviation of returns) in the securities file.
<pre>
    Disperse.exe optimise <a href="Input\TestSecurities1.csv">Input\TestSecurities1.csv</a> <a href="ExpectedOutput\IndependentMinVolatilityPortfolio.csv">Output\IndependentMinVolatilityPortfolio.csv</a> 0 ><a href="ExpectedOutput\IndependentMinVolatilityPortfolioOut.txt">Output\IndependentMinVolatilityPortfolioOut.txt</a>
</pre>

If securities of equal risk are input then the shared risk factors (correlation between the securities) determines the optimised portfolio.
Here two of three securities are specified to depend entirely on the same risk factor (perfectly correlate) so they are treated as being the same security with half the assets being put in the two correlated assets and the other half being allocated to the third, independent security.
<pre>
    Disperse.exe optimise -l <a href="Input\AandBPerfectCorrelation.csv">Input\AandBPerfectCorrelation.csv</a> <a href="Input\TestSecurities2.csv">Input\TestSecurities2.csv</a> <a href="ExpectedOutput\TwoFundsCorrelatedPortfolio.csv">Output\TwoFundsCorrelatedPortfolio.csv</a> 0 ><a href="ExpectedOutput\TwoFundsCorrelatedPortfolioOut.txt">Output\TwoFundsCorrelatedPortfolioOut.txt</a>
</pre>

A group restriction can also be applied. Here the maximum amount which can be put into some of the securities is restricted resulting in an equal weighted portfolio even though some of the assets are riskier than others.
<pre>
    Disperse.exe optimise -g <a href="Input\GroupRestriction.csv">Input\GroupRestriction.csv</a> <a href="Input\TestSecurities1.csv">Input\TestSecurities1.csv</a> <a href="ExpectedOutput\GroupRestrictionPortfolio.csv">Output\GroupRestrictionPortfolio.csv</a> 0 ><a href="ExpectedOutput\GroupRestrictionPortfolioOut.txt">Output\GroupRestrictionPortfolioOut.txt</a>
</pre>
