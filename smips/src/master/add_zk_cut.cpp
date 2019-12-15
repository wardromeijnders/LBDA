#include "master.h"

bool Master::add_zk_cut(double *beta,
                        double gamma,
                        double kappa,
                        double *x,
                        double theta,
                        double tol)
{
    double betaxgamma = gamma;
    for (size_t var = 0; var != d_n1; ++var)
        betaxgamma += beta[var] * x[var];

    std::cout << "kappa = " << kappa << '\n';
    std::cout << "old theta = " << theta
              << ". new theta = " << betaxgamma - (kappa - 1) * theta << ".\n";

    if (betaxgamma > kappa * theta + tol)  // then add cut and return false
    {
        ++d_nSlacks;

        // adding the cut to d_cmodel
        GRBaddvar(d_cmodel,
                  0,
                  NULL,
                  NULL,
                  0,
                  0,
                  1e20,
                  GRB_CONTINUOUS,
                  NULL);  // slack

        size_t numVars = d_n1 + 2;  // theta, x-vars, and slack

        int cind[numVars];
        std::iota(cind, cind + d_n1 + 1, 0);
        cind[d_n1 + 1] = d_n1 + d_nSlacks;

        double cval[numVars];
        cval[0] = kappa;
        for (size_t var = 0; var != d_n1; ++var)
            cval[var + 1] = -beta[var];

        cval[numVars - 1] = -1;  // >= constraint, so slack features with -1
        GRBaddconstr(d_cmodel, numVars, cind, cval, GRB_EQUAL, gamma, NULL);

        // update slack identities
        d_kappa.push_back(kappa);
        d_beta.push_back(std::vector<double>{beta, beta + d_n1});
        d_gamma.push_back(gamma);

        return false;
    }

    return true;  // betaxgamma >= theta, no cut added
}