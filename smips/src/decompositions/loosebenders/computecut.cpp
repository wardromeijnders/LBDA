#include "decompositions/loosebenders.h"
#include "subproblem.h"


LooseBenders::Cut LooseBenders::computeCut(arma::vec const &x)
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

        auto const info = sub.gomInfo();
        double const prob = d_problem.d_scenarioProbabilities[scenario];

        // Gomory is lambda^T (omega - alpha) + psi(omega - alpha), so we add
        // lambda^T alpha.
        arma::vec rhs = omega - d_alpha;
        gamma += prob * computeGomory(scenario, rhs, info.vBasis, info.cBasis);
        gamma += prob * arma::dot(info.lambda, d_alpha);

        dual -= prob * info.lambda;
    }

    return Cut{Tmat * dual, gamma};
}
