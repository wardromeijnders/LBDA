#include "decompositions/loosebenders.h"


LooseBenders::LooseBenders(GRBEnv &env,
                           Problem const &problem,
                           arma::vec const &alpha,
                           double timeLimit) :
    Decomposition(env, problem),
    d_alpha(alpha),
    d_visited(problem.nScenarios()),
    d_objectives(problem.nScenarios())
{
    auto const &Wmat = d_problem.Wmat();

    arma::Col<char> vTypes(Wmat.n_rows);
    vTypes.head(problem.nSecondStageIntVars()).fill(GRB_INTEGER);
    vTypes.tail(Wmat.n_rows - problem.nSecondStageIntVars()).fill(GRB_CONTINUOUS);

    d_vars = d_model.addVars(d_problem.d_secondStageLowerBound.memptr(),
                             d_problem.d_secondStageUpperBound.memptr(),
                             problem.d_secondStageCoeffs.memptr(),
                             vTypes.memptr(),
                             nullptr,
                             Wmat.n_rows);

    arma::Col<char> senses(Wmat.n_cols);
    senses.fill(GRB_GREATER_EQUAL);
    senses.head(d_problem.d_nSecondStageLeqConstraints).fill(GRB_LESS_EQUAL);
    senses
        .tail(Wmat.n_cols - d_problem.d_nSecondStageLeqConstraints
              - d_problem.d_nSecondStageGeqConstraints)
        .fill(GRB_EQUAL);

    GRBLinExpr lhs[Wmat.n_cols];

    for (size_t conIdx = 0; conIdx != Wmat.n_cols; ++conIdx)
        lhs[conIdx].addTerms(Wmat.colptr(conIdx), d_vars, Wmat.n_rows);

    arma::vec rhs = arma::zeros(Wmat.n_cols);

    d_constrs = d_model.addConstrs(lhs,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Wmat.n_cols);

    d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
    d_model.update();
}
