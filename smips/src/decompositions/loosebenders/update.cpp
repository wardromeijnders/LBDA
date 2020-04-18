#include "decompositions/loosebenders.h"

void LooseBenders::update(arma::vec &rhs,
                          arma::Col<int> const &vBasis,
                          arma::Col<int> const &cBasis)
{
    size_t const ss_leq = d_problem.d_nSecondStageLeqConstraints;
    size_t const ss_geq = d_problem.d_nSecondStageGeqConstraints;

    // Relax <= and >= constraints if they are non-binding.
    rhs(arma::find(cBasis.head(ss_leq) == GRB_BASIC)).fill(arma::datum::inf);

    // TODO clean this up
    // >= constraints
    for (size_t con = ss_leq; con != ss_leq + ss_geq; ++con)
        if (cBasis[con] == GRB_BASIC)
            rhs[con] = -arma::datum::inf;

    auto const &Wmat = d_problem.Wmat();

    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);

    arma::vec lb = d_problem.d_secondStageLowerBound;
    arma::vec ub = d_problem.d_secondStageUpperBound;

    // Relax appropriate variable bounds if the bound is not tight.
    lb.elem(arma::find(vBasis != GRB_NONBASIC_LOWER)).fill(-arma::datum::inf);
    ub.elem(arma::find(vBasis != GRB_NONBASIC_UPPER)).fill(arma::datum::inf);

    d_model.set(GRB_DoubleAttr_LB, d_vars, lb.memptr(), Wmat.n_rows);
    d_model.set(GRB_DoubleAttr_UB, d_vars, ub.memptr(), Wmat.n_rows);
}
