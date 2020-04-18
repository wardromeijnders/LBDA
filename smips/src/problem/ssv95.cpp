#include "problem.h"

// TODO is this ssv95 or ssv98?
void Problem::ssv95(size_t S,
                    bool fs_continuous,
                    bool ss_binary,
                    bool standard_T)
{
    clearSub();

    size_t const n1 = 2;
    size_t const n2 = 4;

    d_nFirstStageLeqConstraints = 0;
    d_nFirstStageGeqConstraints = 0;

    d_nSecondStageLeqConstraints = 2;
    d_nSecondStageGeqConstraints = 0;

    d_nFirstStageIntVars = fs_continuous ? 0 : 2;
    d_nSecondStageIntVars = 4;

    size_t nScenarios = S * S;

    d_firstStageLowerBound = arma::zeros(n1);
    d_secondStageLowerBound = arma::zeros(n2);
    d_firstStageUpperBound = std::vector<double>(n1, 5.0);

    double ub = ss_binary ? 1.0 : arma::datum::inf;
    d_secondStageUpperBound = std::vector<double>(n2, ub);

    d_firstStageCoeffs = std::vector<double>{-1.5, -4};
    d_secondStageCoeffs = std::vector<double>{-16, -19, -23, -28};

    d_scenarioProbabilities = std::vector<double>(nScenarios, 1.0 / nScenarios);

    d_L = -320;

    d_Wmat = {{2, 3, 4, 5}, {6, 1, 3, 2}};
    d_Wmat = d_Wmat.t();

    if (standard_T)
        d_Tmat = {{1, 0}, {0, 1}};
    else
        d_Tmat = {{2.0 / 3, 1.0 / 3}, {1.0 / 3, 2.0 / 3}};

    d_Tmat = d_Tmat.t();

    arma::mat omega(nScenarios, 2);

    double jump = 10.0 / (S - 1);
    std::vector<double> omega_vals(S);

    for (size_t s = 0; s != S; ++s)
        omega_vals[s] = 5 + s * jump;

    for (size_t s1 = 0; s1 != S; ++s1)
        for (size_t s2 = 0; s2 != S; ++s2)
        {
            size_t s = s1 * S + s2;
            omega(s, 0) = omega_vals[s1];
            omega(s, 1) = omega_vals[s2];
        }

    d_Amat = arma::mat(n1, 0);
    d_scenarios = omega.t();
}
