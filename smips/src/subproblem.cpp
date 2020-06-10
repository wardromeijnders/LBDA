#include "subproblem.h"


SubProblem::SubProblem(GRBEnv &env, Problem const &problem) :
    d_model(env), d_problem(problem)
{
    auto const &Wmat = d_problem.Wmat();

    arma::Col<char> vTypes(Wmat.n_rows);
    vTypes.fill(GRB_CONTINUOUS);

    d_vars = d_model.addVars(d_problem.d_secondStageLowerBound.memptr(),
                             d_problem.d_secondStageUpperBound.memptr(),
                             d_problem.d_secondStageCoeffs.memptr(),
                             vTypes.memptr(),
                             nullptr,
                             Wmat.n_rows);

    size_t const ss_leq = d_problem.d_nSecondStageLeqConstraints;
    size_t const ss_geq = d_problem.d_nSecondStageGeqConstraints;

    arma::Col<char> senses(Wmat.n_cols);  // constraint senses
    senses.fill(GRB_GREATER_EQUAL);
    senses.head(ss_leq).fill(GRB_LESS_EQUAL);
    senses.tail(Wmat.n_cols - ss_leq - ss_geq).fill(GRB_EQUAL);

    arma::vec rhs = arma::zeros(Wmat.n_cols);

    GRBLinExpr Wy[Wmat.n_cols];  // constraint lhs
    for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
        Wy[iter.col()] += *iter * d_vars[iter.row()];

    d_constrs = d_model.addConstrs(Wy,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Wmat.n_cols);

    d_model.update();
}

SubProblem::SubProblem(SubProblem const &other) :
    d_model(other.d_model), d_problem(other.d_problem)
{
    d_vars = d_model.getVars();
    d_constrs = d_model.getConstrs();
}

SubProblem::~SubProblem()
{
    delete[] d_vars;
    delete[] d_constrs;
}

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

void SubProblem::solve()
{
    d_model.optimize();
}

void SubProblem::update(arma::vec &rhs)
{
    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), rhs.n_elem);
}

void SubProblem::update(arma::vec &&rhs)
{
    update(rhs);
}
