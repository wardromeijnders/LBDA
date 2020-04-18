#include "subproblem.h"


SubProblem::GomInfo const SubProblem::gomInfo()
{
    auto const &Wmat = d_problem.Wmat();

    auto const *lambda = d_model.get(GRB_DoubleAttr_Pi, d_constrs, Wmat.n_cols);
    auto const *vBasis = d_model.get(GRB_IntAttr_VBasis, d_vars, Wmat.n_rows);
    auto const *cBasis = d_model.get(GRB_IntAttr_CBasis, d_constrs, Wmat.n_cols);

    auto gomInfo = GomInfo{arma::vec(lambda, Wmat.n_cols),
                           arma::Col<int>(vBasis, Wmat.n_rows),
                           arma::Col<int>(cBasis, Wmat.n_cols)};

    delete[] lambda;
    delete[] vBasis;
    delete[] cBasis;

    return gomInfo;
}
