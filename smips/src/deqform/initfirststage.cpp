#include "deqform.h"

void DeqForm::initFirstStage(size_t n1,
                             size_t p1,
                             size_t m1,
                             size_t fs_leq,
                             size_t fs_geq,
                             double *lb,
                             double *ub,
                             double *c,
                             double *rhs,
                             arma::mat &Amat)
{
    // variables
    char vTypes[n1];
    std::fill_n(vTypes, p1, GRB_INTEGER);
    std::fill_n(vTypes + p1, n1 - p1, GRB_CONTINUOUS);
    d_xVars = d_model.addVars(lb, ub, c, vTypes, nullptr, n1);

    // constraints
    GRBLinExpr lhsExprs[m1];
    for (size_t conIdx = 0; conIdx != m1; ++conIdx)
        lhsExprs[conIdx].addTerms(Amat.colptr(conIdx), d_xVars, n1);

    char senses[m1];
    std::fill(senses, senses + fs_leq, GRB_LESS_EQUAL);
    std::fill(senses + fs_leq, senses + fs_leq + fs_geq, GRB_GREATER_EQUAL);
    std::fill(senses + fs_leq + fs_geq, senses + m1, GRB_EQUAL);

    GRBConstr *constrs = d_model.addConstrs(lhsExprs, senses, rhs, nullptr, m1);
    delete[] constrs;
}
