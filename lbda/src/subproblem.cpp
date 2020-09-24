#include "subproblem.h"


SubProblem::SubProblem(ProblemData const &problem) :
    d_model(d_env), d_problem(problem)
{
    auto const &Wmat = d_problem.Wmat();

    arma::Col<char> vTypes(Wmat.n_rows);  // TODO allow second-stage int?
    vTypes.fill(GRB_CONTINUOUS);          //  (this is a relaxation instead)

    d_vars = d_model.addVars(d_problem.secondStageLowerBound().memptr(),
                             d_problem.secondStageUpperBound().memptr(),
                             d_problem.secondStageCoeffs().memptr(),
                             vTypes.memptr(),
                             nullptr,
                             Wmat.n_rows);

    GRBLinExpr lhs[Wmat.n_cols];
    for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
        lhs[iter.col()] += *iter * d_vars[iter.row()];  // Wy

    auto const &senses = d_problem.secondStageConstrSenses();
    arma::vec rhs = arma::zeros(Wmat.n_cols);

    d_constrs = d_model.addConstrs(lhs,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Wmat.n_cols);

    d_model.update();
}

SubProblem::~SubProblem()
{
    delete[] d_vars;
    delete[] d_constrs;
}

SubProblem::BasisInfo const SubProblem::basisInfo()
{
    auto const &Wmat = d_problem.Wmat();

    auto const *vBasis = d_model.get(GRB_IntAttr_VBasis, d_vars, Wmat.n_rows);
    auto const *cBasis = d_model.get(GRB_IntAttr_CBasis, d_constrs, Wmat.n_cols);

    auto basisInfo = BasisInfo{arma::Col<int>(vBasis, Wmat.n_rows),
                               arma::Col<int>(cBasis, Wmat.n_cols)};

    delete[] vBasis;
    delete[] cBasis;

    return basisInfo;
}

SubProblem::Duals const SubProblem::duals()
{
    auto const &Wmat = d_problem.Wmat();

    // Shadow prices of the upper bounds on the second-stage variables. This
    // defaults to zeroes in the case we do not have complete recourse. As a
    // dirty fix, we ignore part of the dual objective corresponding to the
    // upper bounds (so we underestimate QLP).
    arma::vec piUb = arma::zeros(Wmat.n_rows);

    auto *vBasisPtr = d_model.get(GRB_IntAttr_VBasis, d_vars, Wmat.n_rows);
    auto const vBasis = arma::Col<int>(vBasisPtr, Wmat.n_rows);

    // SAObjUp is the largest objective value for which the current basis
    // remains optimal.
    auto *piUbPtr = d_model.get(GRB_DoubleAttr_SAObjUp, d_vars, Wmat.n_rows);

    // If the variable is at the upper bound, the shadow price is equal to
    // the reduced costs. Else the shadow price is zero.
    piUb = d_problem.secondStageCoeffs() - arma::vec(piUbPtr, Wmat.n_rows);
    piUb(arma::find(vBasis != GRB_NONBASIC_UPPER)).fill(0.);

    auto const *lambda = d_model.get(GRB_DoubleAttr_Pi, d_constrs, Wmat.n_cols);
    auto duals = Duals{arma::vec{lambda, Wmat.n_cols}, piUb};

    delete[] vBasisPtr;
    delete[] piUbPtr;
    delete[] lambda;

    return duals;
}

void SubProblem::solve()
{
    d_model.optimize();

    if (d_model.get(GRB_IntAttr_Status) != GRB_OPTIMAL)
        throw std::runtime_error("Sub problem is infeasible: do we have "
                                 "(relatively) complete recourse?");
}

void SubProblem::updateRhs(arma::vec &rhs)
{
    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), rhs.size());
}

void SubProblem::updateRhs(arma::vec &&rhs)
{
    updateRhs(rhs);
}
