#include "cutfamilies/strongbenders.h"

#include "subproblem.h"


StrongBenders::StrongBenders(ProblemData const &problem) : CutFamily(problem)
{
    // Free first-stage variables for Lagrangian relaxation.
    d_zVars = d_model.addVars(problem.firstStageLowerBound().memptr(),
                              problem.firstStageUpperBound().memptr(),
                              nullptr,
                              problem.firstStageVarTypes().memptr(),
                              nullptr,
                              problem.firstStageCoeffs().n_elem);

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
        lhs[iter.col()] += *iter * d_zVars[iter.row()];  // Tz

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
    delete[] d_zVars;
    delete[] d_constrs;
}

StrongBenders::Cut StrongBenders::computeCut(arma::vec const &x)
{
    auto const &Tmat = d_problem.Tmat();

    arma::vec Tx = Tmat.t() * x;
    arma::vec beta = arma::zeros(Tmat.n_rows);

    double gamma = 0;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        d_sub.updateRhs(omega - Tx);
        d_sub.solve();

        auto const duals = d_sub.duals();
        double const prob = d_problem.scenarioProbability(scenario);

        arma::vec pi = Tmat * duals.lambda;

        update(omega, pi);
        d_model.optimize();

        gamma += prob * d_model.get(GRB_DoubleAttr_ObjVal);
        beta -= prob * pi;
    }

    return Cut{beta, gamma};
}

void StrongBenders::update(arma::vec &rhs, arma::vec &pi)
{
    d_model.set(GRB_DoubleAttr_Obj,
                d_zVars,
                pi.memptr(),
                d_problem.firstStageCoeffs().n_elem);

    auto const &Wmat = d_problem.Wmat();
    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);
}
