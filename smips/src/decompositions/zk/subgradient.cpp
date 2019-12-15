#include "decompositions/zk.h"

void ZK::subgradient(double &alpha, double *beta, double &kappa, double weight)
{
    double lambda[d_nConstrs];
    GRBgetdblattrarray(d_model, "Pi", 0, d_nConstrs, lambda);

    for (size_t con = 0; con != d_nConstrs; ++con)
    {
        alpha += weight * lambda[con] * d_omega[con];
        kappa += weight * lambda[con] * d_tau[con];

        std::vector<double> &Trow = d_Tmat[con];

        for (size_t var = 0; var != d_n1; ++var)
        {
            if (var < Trow.size())
                beta[var] -= weight * lambda[con] * Trow[var];
        }
    }
}