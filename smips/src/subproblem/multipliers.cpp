#include "subproblem.h"


SubProblem::Multipliers const SubProblem::multipliers()
{
    auto const &Wmat = d_problem.Wmat();

    // Shadow prices of the upper bounds on the second-stage variables. This
    // defaults to zeroes in the case we do not have complete recourse. As a
    // dirty fix, we ignore part of the dual objective corresponding to the
    // upper bounds (so we underestimate QLP).
    arma::vec piUb = arma::zeros(Wmat.n_rows);

    if (d_model.get(GRB_IntAttr_Status) != GRB_INFEASIBLE)
    {
        auto *vBasisPtr = d_model.get(GRB_IntAttr_VBasis, d_vars, Wmat.n_rows);
        auto const vBasis = arma::Col<int>(vBasisPtr, Wmat.n_rows);

        // SAObjUp is the largest objective value for which the current basis
        // remains optimal.
        auto *piUbPtr = d_model.get(GRB_DoubleAttr_SAObjUp, d_vars, Wmat.n_rows);

        // If the variable is at the upper bound, the shadow price is equal to
        // the reduced costs. Else the shadow price is zero.
        piUb = d_problem.d_secondStageCoeffs - arma::vec(piUbPtr, Wmat.n_rows);
        piUb(arma::find(vBasis != GRB_NONBASIC_UPPER)).fill(0.);

        delete[] vBasisPtr;
        delete[] piUbPtr;
    }

    auto const *lambda = d_model.get(GRB_DoubleAttr_Pi, d_constrs, Wmat.n_cols);
    auto multipliers = Multipliers{arma::vec{lambda, Wmat.n_cols}, piUb};

    delete[] lambda;

    return multipliers;
}
