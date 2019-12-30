#pragma once
#include <vector>

enum class Objective
{
	MAXIMISE_RETURN,
	MINIMISE_RISK
};


// The limit is the maximum allowed risk or minimum allowed return
void solve(Objective objective, double limit, const std::vector<Security> &securities, 