#include "benders.h"

void Benders::lpCut(double *x, double *beta, double &gamma)
{
    auto &omega = d_problem.d_omega;
    auto &Tmat = d_problem.d_Tmat;
    auto &probs = d_problem.d_probs;

    gamma = 0;

    double Tx[d_m2];
    computeTx(x, Tx);

    double dual[d_m2];
    std::fill(dual, dual + d_m2, 0.0);

    for (size_t s = 0; s != d_S; ++s)
    {
        double *ws = omega[s].data();  // scenario (c-style array pointer)

        double rhs[d_m2];                         // rhs vector (c-style array)
        for (size_t row = 0; row != d_m2; ++row)  // compute element-by-element
            rhs[row] = ws[row] - Tx[row];

        d_sub.update(rhs);
        d_sub.solve();

        auto const info = d_sub.multipliers();

        double *lambda = info.lambda;
        double *pi_u = info.pi_u;
        double prob = probs[s];

        for (size_t row = 0; row != d_m2; ++row)
        {
            dual[row] -= prob * lambda[row];
            gamma += prob * lambda[row] * ws[row];
        }

        for (size_t var = 0; var != d_n2; ++var)
            gamma += prob * pi_u[var] * d_problem.d_u2[var];

        delete[] lambda;
        delete[] pi_u;
    }

    for (size_t col = 0; col != d_n1; ++col)
    {
        beta[col] = 0.0;
        for (size_t row = 0; row != d_m2; ++row)
            beta[col] += dual[row] * Tmat[row][col];
    }
}