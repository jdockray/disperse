mkdir Output

Disperse.exe help >Output\GeneralHelp.txt
Disperse.exe help combine >Output\CombineCommandHelp.txt
Disperse.exe help multiply >Output\MultiplyCommandHelp.txt
Disperse.exe help optimise >Output\OptimiseCommandHelp.txt

Disperse.exe multiply -m Input\FundRegionWeightings.csv,Input\RegionSectorWeightings.csv -r Output\FundSectorWeightings.csv
Disperse.exe multiply -m Input\FundSectorWeightings.csv -s 0.1 -r Output\FundSectorEffect.csv
Disperse.exe multiply -l Input\ShareOverlap.csv -s 0.05 -r Output\ShareOverlapEffect.csv
Disperse.exe combine -m Input\FundSectorEffect.csv,Input\ShareOverlapEffect.csv -a 0.85 -r Output\CombinedExposures.csv
Disperse.exe optimise -m Input\CombinedExposures.csv -f Output\OutputFactorWeightings.csv Input\EquityFunds.csv Output\OutputPortfolio.csv 0.09 >Output\OptimisationOut.txt

Disperse.exe optimise Input\TestSecurities1.csv Output\IndependentMinVolatilityPortfolio.csv 0 >Output\IndependentMinVolatilityPortfolioOut.txt
Disperse.exe optimise -l Input\AandBPerfectCorrelation.csv Input\TestSecurities2.csv Output\TwoFundsCorrelatedPortfolio.csv 0 >Output\TwoFundsCorrelatedPortfolioOut.txt
Disperse.exe optimise -g Input\GroupRestriction.csv Input\TestSecurities1.csv Output\GroupRestrictionPortfolio.csv 0 >Output\GroupRestrictionPortfolioOut.txt

git diff Output ExpectedOutput
