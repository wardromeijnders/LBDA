#include "decompositions/strongbenders.h"

#include "subproblem.h"


StrongBenders::StrongBenders(GRBEnv &env, Problem const &problem) :
    Decomposition(env, problem)
{
    auto const &Amat = problem.Amat();

    // adding first-stage variables (z)
    d_z_vars = d_model.addVars(problem.d_firstStageLowerBound.memptr(),
                               problem.d_firstStageUpperBound.memptr(),
                               nullptr,
                               problem.firstStageVarTypes().memptr(),
                               nullptr,
                               Amat.n_rows);

    // TODO: include first-stage constraints

    auto const &Wmat = problem.Wmat();

    // adding second-stage variables (y)
    GRBVar *y_vars = d_model.addVars(problem.d_secondStageLowerBound.memptr(),
                                     problem.d_secondStageUpperBound.memptr(),
                                     problem.secondStageCoeffs().memptr(),
                                     problem.secondStageVarTypes().memptr(),
                                     nullptr,
                                     Wmat.n_rows);

    auto const &Tmat = d_problem.Tmat();

    // constraint lhs
    GRBLinExpr TxWy[Tmat.n_cols];

    for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
        TxWy[iter.col()] += *iter * y_vars[iter.row()];

    for (auto iter = Tmat.begin(); iter != Tmat.end(); ++iter)
        TxWy[iter.col()] += *iter * d_z_vars[iter.row()];

    auto const &senses = d_problem.secondStageConstrSenses();
    arma::vec rhs = arma::zeros(Tmat.n_cols);

    d_constrs = d_model.addConstrs(TxWy,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Tmat.n_cols);

    d_model.update();

    delete[] y_vars;
}

StrongBenders::~StrongBenders()
{
    delete[] d_z_vars;
    delete[] d_constrs;
}

StrongBenders::Cut StrongBenders::computeCut(arma::vec const &x)
{
    auto const &Tmat = d_problem.Tmat();

    arma::vec Tx = Tmat * x;
    arma::vec beta = arma::zeros(Tmat.n_rows);

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        sub.update(omega - Tx);
        sub.solve();

        auto const info = sub.multipliers();
        double const prob = d_problem.probability(scenario);

        arma::vec pi = Tmat * info.lambda;

        update(omega, pi);

        gamma += prob * solve();
        beta -= prob * pi;
    }

    return Cut{beta, gamma};
}

void StrongBenders::update(arma::vec &rhs, arma::vec &pi)
{
    auto const &Amat = d_problem.Amat();
    d_model.set(GRB_DoubleAttr_Obj, d_z_vars, pi.memptr(), Amat.n_rows);

    auto const &Wmat = d_problem.Wmat();
    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);
}
