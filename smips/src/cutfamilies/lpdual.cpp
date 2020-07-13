#include "cutfamilies/lpdual.h"

#include "subproblem.h"

LpDual::LpDual(GRBEnv &env, Problem const &problem) : CutFamily(env, problem)
{
}

LpDual::Cut LpDual::computeCut(arma::vec const &x)
{
    auto const &Tmat = d_problem.Tmat();

    arma::vec Tx = Tmat.t() * x;
    arma::vec dual = arma::zeros(Tmat.n_cols);  // decomposition coeffs

    auto sub = SubProblem(d_env, d_problem);

    double obj = 0.;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        sub.updateRhs(omega - Tx);
        sub.solve();

        auto const duals = sub.duals();
        double const prob = d_problem.probability(scenario);

        dual += prob * duals.lambda;
        obj += prob * sub.objective();
    }

    arma::vec beta = Tmat * dual;
    double gamma = obj + arma::dot(beta, x);

    return Cut{-beta, gamma};
}
