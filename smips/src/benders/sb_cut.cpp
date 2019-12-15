#include "benders.h"

void Benders::sb_cut(double *x, double *beta, double &gamma)
{
    auto &omega = d_problem.d_omega;
    auto &Tmat = d_problem.d_Tmat;
    auto &probs = d_problem.d_probs;

    double Tx[d_m2];
    computeTx(x, Tx);

    gamma = 0.0;

    double lw = 0;
    for (size_t s = 0; s != d_S; ++s)
    {
        double *ws = omega[s].data();  // scenario (c-style array pointer)
        double prob = probs[s];

        double rhs[d_m2];  // rhs vector (c-style array)

        for (size_t row = 0; row != d_m2; ++row)  // compute element-by-element
            rhs[row] = ws[row] - Tx[row];

        d_sub.update(rhs);
        Sub::Multipliers info = d_sub.solve();
        double *lambda = info.lambda;
        double *pi_u = info.pi_u;

        double pi[d_n1];  // pi = lambda T
        for (size_t var = 0; var != d_n1; ++var)
        {
            pi[var] = 0.0;
            for (size_t row = 0; row != d_m2; ++row)
                pi[var] += lambda[row] * Tmat[row][var];
            beta[var] += -prob * pi[var];
        }

        d_lr.update(ws, pi);
        double Lpiw = d_lr.solve();
        gamma += prob * Lpiw;

        for (size_t row = 0; row != d_m2; ++row)
            lw += prob * lambda[row] * ws[row];

        delete[] lambda;
        delete[] pi_u;
    }

    std::cout << "lw =  " << lw << ". L(pi, w) = " << gamma << '\n';
}