#include "cuts/strongbenders.h"
#include "subproblem.h"


StrongBenders::CutResult StrongBenders::computeCut(arma::vec const &x)
{
    arma::vec Tx = d_problem.d_Tmat * x;
    arma::vec beta = arma::zeros(d_problem.d_n1);

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;
    double lw = 0;

    for (size_t s = 0; s != d_problem.d_S; ++s)
    {
        arma::vec omega = d_problem.d_omega.col(s);

        sub.update(omega - Tx);
        sub.solve();

        auto const info = sub.multipliers();
        double const prob = d_problem.d_probs[s];

        arma::vec pi = d_problem.d_Tmat * info.lambda;

        d_lr.update(omega, pi);
        gamma += prob * d_lr.solve();

        beta -= prob * pi;

        lw += prob * arma::dot(info.lambda, omega);
    }

    std::cout << "lw = " << lw << ". L(pi, w) = " << gamma << '\n';
    return CutResult{beta, gamma};
}
