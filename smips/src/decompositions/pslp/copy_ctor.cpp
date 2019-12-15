#include "decompositions/pslp.h"

Pslp::Pslp(const Pslp &other) :
    d_n1(other.d_n1),
    d_S(other.d_S),
    d_nCuts(other.d_nCuts),
    d_model(other.d_model),
    d_kappa(other.d_kappa),
    d_gamma(other.d_gamma),
    d_nu(other.d_nu),
    d_probs(other.d_probs)
{
    GRBVar *vars = d_model.getVars();
    d_z = vars[0];
    d_eta = vars[1];
    delete[] vars;

    GRBConstr *constrs = d_model.getConstrs();

    for (size_t con = 0; con != other.d_constrs.size(); ++con)
        d_constrs.push_back(constrs[con]);

    delete[] constrs;

    d_zk.reserve(d_S);

    for (size_t s = 0; s != d_S; ++s)
        d_zk.push_back(other.d_zk[s]);
}