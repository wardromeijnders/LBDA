#include "decompositions/pslp.h"
void Pslp::strong_zk_cut(double *x,
                         double theta,
                         Master &master,
                         double *beta,
                         double &gamma,
                         double &kappa,
                         size_t maxRounds)
{
    update_rhs(x, theta);
    l_shaped(master, maxRounds);

    double *mu = d_model.get(GRB_DoubleAttr_Pi,
                             d_constrs.data(),
                             d_nCuts);  // extract dual multipliers
    // compute cut coefficients
    kappa = 1;
    gamma = 0;
    std::fill_n(beta, d_n1, 0);

    for (size_t cut = 0; cut != d_nCuts; ++cut)
    {
        double muval = mu[cut];

        kappa += muval * d_nu[cut];
        gamma += muval * d_gamma[cut];

        for (size_t var = 0; var != d_n1; ++var)
            beta[var] += muval * d_kappa[cut][var];
    }
}
