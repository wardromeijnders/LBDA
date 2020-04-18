#include "masterproblem.h"

void MasterProblem::addCut(Decomposition::Cut &cut)
{
    ++d_nSlacks;

    GRBaddvar(d_cmodel,  // new slack variable
              0,
              nullptr,
              nullptr,
              0,
              0,
              arma::datum::inf,
              GRB_CONTINUOUS,
              nullptr);

    size_t const n1 = d_problem.Amat().n_rows;

    auto cind = arma::regspace<arma::Col<int>>(0, n1 + 2);
    cind[n1 + 1] = n1 + d_nSlacks;  // refers to the new slack variable.

    arma::vec cval(n1 + 2);
    cval.subvec(1, n1) = -cut.beta;
    cval[0] = 1;        // TODO magic numbers (maybe?)
    cval[n1 + 1] = -1;  // >= constraint, so slack features with -1

    GRBaddconstr(d_cmodel,
                 n1 + 2,
                 cind.memptr(),
                 cval.memptr(),
                 GRB_EQUAL,
                 cut.gamma,
                 nullptr);
}
