#include "decompositions/pslp.h"

void Pslp::zk_cut(double *x,
                  double theta,
                  Master &master,
                  double *beta,
                  double &gamma,
                  double &kappa,
                  size_t maxRounds)
{
    kappa = 1;
    gamma = 0;

    std::fill_n(beta, d_n1, 0);

    // TODO: range based for loop?
    for (size_t scenario = 0; scenario != d_S; ++scenario)
    {
        d_zk[scenario].update(x, theta);
        d_zk[scenario].solve(x,
                             theta,
                             master.d_cmodel,
                             master.d_kappa,
                             master.d_beta,
                             master.d_gamma,
                             maxRounds);

        d_zk[scenario].subgradient(gamma, beta, kappa, d_probs[scenario]);
    }
}