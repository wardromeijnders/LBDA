#include "decompositions/pslp.h"

void Pslp::update_rhs(double *x, double theta)
{
    d_x = x;
    d_theta = theta;

    double rhs[d_nCuts];

    for (size_t cut = 0; cut != d_nCuts; ++cut)
    {
        rhs[cut] = d_gamma[cut] - d_nu[cut] * theta;

        for (size_t var = 0; var != d_n1; ++var)
            rhs[cut] += d_kappa[cut][var] * x[var];
    }

    d_model.set(GRB_DoubleAttr_RHS, d_constrs.data(), rhs, d_nCuts);
}