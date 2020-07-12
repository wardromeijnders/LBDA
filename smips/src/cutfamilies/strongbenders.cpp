#include "cutfamilies/strongbenders.h"

#include "subproblem.h"


StrongBenders::StrongBenders(GRBEnv &env, Problem const &problem) :
    CutFamily(env, problem)
{
    auto const &Amat = problem.Amat();

    // Free first-stage variables for Lagrangian relaxation.
    d_xVars = d_model.addVars(problem.firstStageLowerBound().memptr(),
                              problem.firstStageUpperBound().memptr(),
                              nullptr,
                              problem.firstStageVarTypes().memptr(),
                              nullptr,
                              Amat.n_rows);

    // TODO: include first-stage constraints?

    auto const &Wmat = problem.Wmat();

    GRBVar *yVars = d_model.addVars(problem.secondStageLowerBound().memptr(),
                                    problem.secondStageUpperBound().memptr(),
                                    problem.secondStageCoeffs().memptr(),
                                    problem.secondStageVarTypes().memptr(),
                                    nullptr,
                                    Wmat.n_rows);

    auto const &Tmat = d_problem.Tmat();

    GRBLinExpr lhs[Tmat.n_cols];  // constraint lhs

    for (auto iter = Tmat.begin(); iter != Tmat.end(); ++iter)
        lhs[iter.col()] += *iter * d_xVars[iter.row()];  // Tx

    for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
        lhs[iter.col()] += *iter * yVars[iter.row()];  // Wy

    auto const &senses = d_problem.secondStageConstrSenses();
    arma::vec rhs = arma::zeros(Tmat.n_cols);

    d_constrs = d_model.addConstrs(lhs,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Tmat.n_cols);

    d_model.update();

    delete[] yVars;
}

StrongBenders::~StrongBenders()
{
    delete[] d_xVars;
    delete[] d_constrs;
}

StrongBenders::Cut StrongBenders::computeCut(arma::vec const &x)
{
    auto const &Tmat = d_problem.Tmat();

    arma::vec Tx = Tmat.t() * x;
    arma::vec beta = arma::zeros(Tmat.n_rows);

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        sub.updateRhs(omega - Tx);
        sub.solve();

        auto const duals = sub.duals();
        double const prob = d_problem.probability(scenario);

        arma::vec pi = Tmat * duals.lambda;

        update(omega, pi);

        gamma += prob * solve();
        beta -= prob * pi;
    }

    return Cut{beta, gamma};
}

void StrongBenders::update(arma::vec &rhs, arma::vec &pi)
{
    auto const &Amat = d_problem.Amat();
    d_model.set(GRB_DoubleAttr_Obj, d_xVars, pi.memptr(), Amat.n_rows);

    auto const &Wmat = d_problem.Wmat();
    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);
}
