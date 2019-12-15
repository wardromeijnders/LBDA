#include "decompositions/zk.h"

void ZK::add_cut(double *coef_x,
                 double *coef_y,
                 double coef_theta,
                 double coef_rhs,
                 double *x,
                 double theta,
                 size_t nxVars,
                 size_t conIdx)
{
    // updating cut coefficients
    d_omega.push_back(coef_rhs);
    d_Tmat.push_back(std::vector<double>{coef_x, coef_x + d_n1});
    d_tau.push_back(coef_theta);

    // adding cut
    // computing rhs
    double rhs = coef_rhs - coef_theta * theta;
    for (size_t var = 0; var != d_n1; ++var)
        rhs -= coef_x[var] * x[var];

    int cind[d_nVars];  // variable indices
    std::iota(cind, cind + d_nVars, 0);
    GRBaddconstr(d_model, d_nVars, cind, coef_y, GRB_EQUAL, rhs, NULL);

    // adding slack
    int vind[1];
    vind[0] = conIdx;
    double vval[] = {-1};
    GRBaddvar(d_model, 1, vind, vval, 0, 0, 1e20, GRB_CONTINUOUS, NULL);
}
