#include "cuts/lpdual.h"
#include "subproblem.h"


LpDual::CutResult LpDual::computeCut(arma::vec const &x)
{
    arma::vec Tx = d_problem.d_Tmat * x;
    arma::vec dual = arma::zeros(d_problem.d_m2);  // cut coefficients

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;

    for (size_t s = 0; s != d_problem.d_S; ++s)
    {
        arma::vec omega = d_problem.d_omega.col(s);


        sub.update(omega - Tx);
        sub.solve();

        auto const info = sub.multipliers();
        double const prob = d_problem.d_probs[s];

        dual -= prob * info.lambda;
        gamma += prob * arma::dot(info.lambda, omega);
        gamma += prob * arma::dot(info.pi_u, d_problem.d_u2);
    }

    return CutResult{d_problem.d_Tmat * dual, gamma};
}
