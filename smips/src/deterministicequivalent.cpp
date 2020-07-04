#include "deterministicequivalent.h"


DeterministicEquivalent::DeterministicEquivalent(GRBEnv &env,
                                                 Problem const &problem) :
    d_problem(problem), d_model(GRBModel(env)), d_status(status::UNSOLVED)
{
    initFirstStage();
    initSecondStage();
}

DeterministicEquivalent::~DeterministicEquivalent()
{
    delete[] d_xVars;
}

void DeterministicEquivalent::initFirstStage()
{
    auto const &Amat = d_problem.Amat();

    d_xVars = d_model.addVars(d_problem.firstStageLowerBound().memptr(),
                              d_problem.firstStageUpperBound().memptr(),
                              d_problem.firstStageCoeffs().memptr(),
                              d_problem.firstStageVarTypes().memptr(),
                              nullptr,
                              Amat.n_rows);

    GRBLinExpr lhs[Amat.n_cols];
    for (auto iter = Amat.begin(); iter != Amat.end(); ++iter)
        lhs[iter.col()] += *iter * d_xVars[iter.row()];

    auto const &senses = d_problem.firstStageConstrSenses();
    GRBConstr *constrs = d_model.addConstrs(lhs,
                                            senses.memptr(),
                                            d_problem.firstStageRhs().memptr(),
                                            nullptr,
                                            Amat.n_cols);

    delete[] constrs;
}

void DeterministicEquivalent::initSecondStage()
{
    auto const &Tmat = d_problem.Tmat();
    auto const &Wmat = d_problem.Wmat();

    GRBLinExpr Tx[Tmat.n_cols];
    for (auto iter = Tmat.begin(); iter != Tmat.end(); ++iter)
        Tx[iter.col()] += *iter * d_xVars[iter.row()];

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        double const prob = d_problem.probability(scenario);
        arma::vec const costs = prob * d_problem.secondStageCoeffs();

        // add variables
        GRBVar *yVars = d_model
                            .addVars(d_problem.secondStageLowerBound().memptr(),
                                     d_problem.secondStageUpperBound().memptr(),
                                     costs.memptr(),
                                     d_problem.secondStageVarTypes().memptr(),
                                     nullptr,
                                     Wmat.n_rows);

        // lhs expression of second-stage constraints, including Wy
        for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
            Tx[iter.col()] += *iter * yVars[iter.row()];

        auto const &senses = d_problem.secondStageConstrSenses();
        auto const omega = d_problem.scenarios().colptr(scenario);

        GRBConstr *constrs = d_model.addConstrs(Tx,
                                                senses.memptr(),
                                                omega,
                                                nullptr,
                                                Wmat.n_cols);

        delete[] yVars;
        delete[] constrs;
    }
}

std::unique_ptr<arma::vec> DeterministicEquivalent::solve(double timeLimit)
{
    d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
    d_model.optimize();

    int status = d_model.get(GRB_IntAttr_Status);

    if (status == GRB_INFEASIBLE)
    {
        d_status = status::INFEASIBLE;
        return std::make_unique<arma::vec>(nullptr);
    }

    d_status = status::SOLVED;

    auto const &Amat = d_problem.Amat();
    auto const *xPtr = d_model.get(GRB_DoubleAttr_X, d_xVars, Amat.n_rows);

    auto result = std::make_unique<arma::vec>(xPtr, Amat.n_rows);
    delete[] xPtr;

    return result;
}
