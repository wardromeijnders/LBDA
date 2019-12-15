#include "decompositions/zk.h"

void ZK::transform_cut(double *coef_x,
                       double &coef_theta,
                       double &coef_rhs,
                       std::vector<double> &kappa,
                       std::vector<std::vector<double>> &beta,
                       std::vector<double> &gamma,
                       size_t nSlacks)
{
    for (size_t slack = 0; slack != nSlacks; ++slack)
    {
        size_t xvar = d_n1 + slack;
        coef_theta += coef_x[xvar] * kappa[slack];

        for (size_t var = 0; var != d_n1; ++var)
            coef_x[var] -= coef_x[xvar] * beta[slack][var];

        // += because rhs is on the rhs
        coef_rhs += coef_x[xvar] * gamma[slack];

        coef_x[xvar] = 0;
    }
}