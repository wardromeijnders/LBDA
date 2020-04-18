#include "subproblem.h"

SubProblem::SubProblem(GRBEnv &env, Problem const &problem) :
    d_model(env),
    d_problem(problem)
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
    for (size_t conIdx = 0; conIdx != Wmat.n_cols; ++conIdx)
        Wy[conIdx].addTerms(Wmat.colptr(conIdx), d_vars, Wmat.n_rows);

    d_constrs = d_model.addConstrs(Wy,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Wmat.n_cols);

    d_model.update();
}
