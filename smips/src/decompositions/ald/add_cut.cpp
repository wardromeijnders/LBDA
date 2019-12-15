#include "decompositions/ald.h"


void Ald::add_cut(size_t nVars,
                  size_t nCuts,
                  double *coef_x,
                  double *coef_y,
                  double coef_eta,
                  double coef_rhs,
                  std::vector<std::vector<double>> &Tmat,
                  std::vector<double> &tau,
                  std::vector<double> &omega)
{
    GRBaddvar(d_model,
              0,
              NULL,
              NULL,
              0.0,
              0.0,
              1e20,
              GRB_CONTINUOUS,
              NULL);  // slack variable

    int cind[nVars + 1];

    // fills cind with 0,1,...,nVars-1
    std::iota(cind, cind + nVars, 0);

    // final element of nVars corresponds to slack variable
    cind[nVars] = nVars + nCuts;

    double cval[nVars + 1];
    std::copy(coef_y, coef_y + nVars, cval);
    cval[nVars] = -1;

    GRBaddconstr(d_model, nVars + 1, cind, cval, GRB_EQUAL, 1, NULL);

    // appending cut to T and tau and omega, for future cuts
    std::vector<double> cut_row(coef_x, coef_x + d_n1);

    Tmat.push_back(cut_row);
    tau.push_back(coef_eta);
    omega.push_back(coef_rhs);
}
