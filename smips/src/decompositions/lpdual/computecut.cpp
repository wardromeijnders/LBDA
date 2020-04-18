#include "decompositions/lpdual.h"
#include "subproblem.h"


LpDual::Cut LpDual::computeCut(arma::vec const &x)
{
    auto const &Tmat = d_problem.Tmat();

    arma::vec Tx = Tmat * x;
    arma::vec dual = arma::zeros(Tmat.n_cols);  // decomposition coeffs

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        sub.update(omega - Tx);
        sub.solve();

        auto const info = sub.multipliers();
        double const prob = d_problem.d_scenarioProbabilities[scenario];

        gamma += prob * arma::dot(info.lambda, omega);
        gamma += prob * arma::dot(info.pi_u, d_problem.d_secondStageUpperBound);

        dual -= prob * info.lambda;
    }

    return Cut{Tmat * dual, gamma};
}
