#include "deterministicequivalent.h"


DeterministicEquivalent::DeterministicEquivalent(ProblemData const &problem) :
    d_problem(problem), d_model(d_env)
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

    auto *constrs = d_model
                        .addConstrs(lhs,
                                    d_problem.firstStageConstrSenses().memptr(),
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

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        double const prob = d_problem.scenarioProbability(scenario);
        arma::vec const costs = prob * d_problem.secondStageCoeffs();

        GRBLinExpr lhs[Tmat.n_cols];

        for (auto iter = Tmat.begin(); iter != Tmat.end(); ++iter)
            lhs[iter.col()] += *iter * xVars[iter.row()];  // Tx

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

        auto *constrs = d_model.addConstrs(lhs,
                                           senses.memptr(),
                                           rhs,
                                           nullptr,
                                           Wmat.n_cols);

        delete[] yVars;
        delete[] constrs;
    }

    delete[] xVars;

    d_model.update();
}

std::unique_ptr<arma::vec> DeterministicEquivalent::solve(double timeLimit)
{
    d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
    d_model.optimize();

    auto status = d_model.get(GRB_IntAttr_Status);
    auto nSolutions = d_model.get(GRB_IntAttr_SolCount);

    // Time limit reached, and no feasible solution is available.
    if (status == GRB_TIME_LIMIT && nSolutions == 0)
        throw std::runtime_error("Time limit exceeded; no feasible solutions.");

    if (isOptimal() || nSolutions > 0)
    {
        auto const &Amat = d_problem.Amat();

        auto const *vars = d_model.getVars();
        auto const *xPtr = d_model.get(GRB_DoubleAttr_X, vars, Amat.n_rows);

        auto result = std::make_unique<arma::vec>(xPtr, Amat.n_rows);

        delete[] vars;
        delete[] xPtr;

        return result;
    }

    throw std::runtime_error("Deterministic equivalent is infeasible.");
}

double DeterministicEquivalent::firstStageObjective()
{
    auto const &Amat = d_problem.Amat();

    auto const *vars = d_model.getVars();
    auto const *xPtr = d_model.get(GRB_DoubleAttr_X, vars, Amat.n_rows);

    arma::vec x(xPtr, Amat.n_rows);

    delete[] vars;
    delete[] xPtr;

    return arma::dot(d_problem.firstStageCoeffs(), x);
}

double DeterministicEquivalent::secondStageObjective()
{
    return objective() - firstStageObjective();
}

bool DeterministicEquivalent::isOptimal()
{
    return d_model.get(GRB_IntAttr_Status) == GRB_OPTIMAL;
}

double DeterministicEquivalent::mipGap()
{
    return 100 * d_model.get(GRB_DoubleAttr_MIPGap);
}
