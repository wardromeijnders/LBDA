#include "deterministicequivalent.h"


DeterministicEquivalent::DeterministicEquivalent(GRBEnv &env,
                                                 Problem const &problem) :
    d_problem(problem), d_model(env)
{
    initFirstStage();
    initSecondStage();
}

void DeterministicEquivalent::initFirstStage()
{
    auto const &Amat = d_problem.Amat();

    auto *xVars = d_model.addVars(d_problem.firstStageLowerBound().memptr(),
                                  d_problem.firstStageUpperBound().memptr(),
                                  d_problem.firstStageCoeffs().memptr(),
                                  d_problem.firstStageVarTypes().memptr(),
                                  nullptr,
                                  Amat.n_rows);

    GRBLinExpr lhs[Amat.n_cols];
    for (auto iter = Amat.begin(); iter != Amat.end(); ++iter)
        lhs[iter.col()] += *iter * xVars[iter.row()];

    auto const &senses = d_problem.firstStageConstrSenses();
    auto *constrs = d_model.addConstrs(lhs,
                                       senses.memptr(),
                                       d_problem.firstStageRhs().memptr(),
                                       nullptr,
                                       Amat.n_cols);

    delete[] xVars;
    delete[] constrs;

    d_model.update();
}

void DeterministicEquivalent::initSecondStage()
{
    auto const &Tmat = d_problem.Tmat();
    auto const &Wmat = d_problem.Wmat();

    auto *xVars = d_model.getVars();

    GRBLinExpr lhs[Tmat.n_cols];
    for (auto iter = Tmat.begin(); iter != Tmat.end(); ++iter)
        lhs[iter.col()] += *iter * xVars[iter.row()];  // Tx

    delete[] xVars;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        double const prob = d_problem.probability(scenario);
        arma::vec const costs = prob * d_problem.secondStageCoeffs();

        // scenario-specific variables
        auto *yVars = d_model.addVars(d_problem.secondStageLowerBound().memptr(),
                                      d_problem.secondStageUpperBound().memptr(),
                                      costs.memptr(),
                                      d_problem.secondStageVarTypes().memptr(),
                                      nullptr,
                                      Wmat.n_rows);

        for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
            lhs[iter.col()] += *iter * yVars[iter.row()];  // Wy

        auto const &senses = d_problem.secondStageConstrSenses();
        auto const rhs = d_problem.scenarios().colptr(scenario);

        GRBConstr *constrs = d_model.addConstrs(lhs,
                                                senses.memptr(),
                                                rhs,
                                                nullptr,
                                                Wmat.n_cols);

        delete[] yVars;
        delete[] constrs;
    }

    d_model.update();
}

std::unique_ptr<arma::vec> DeterministicEquivalent::solve(double timeLimit)
{
    d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
    d_model.optimize();

    if (d_model.get(GRB_IntAttr_Status) != GRB_OPTIMAL)
        throw std::runtime_error("Deterministic equivalent is infeasible.");

    auto const &Amat = d_problem.Amat();

    auto const *vars = d_model.getVars();
    auto const *xPtr = d_model.get(GRB_DoubleAttr_X, vars, Amat.n_rows);

    auto result = std::make_unique<arma::vec>(xPtr, Amat.n_rows);

    delete[] vars;
    delete[] xPtr;

    return result;
}
