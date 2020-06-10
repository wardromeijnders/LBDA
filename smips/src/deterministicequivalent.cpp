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
    // TODO clean this up
    auto const &Amat = d_problem.Amat();

    // variables
    char vTypes[Amat.n_rows];
    std::fill_n(vTypes, d_problem.nFirstStageIntVars(), GRB_INTEGER);
    std::fill_n(vTypes + d_problem.nFirstStageIntVars(),
                Amat.n_rows - d_problem.nFirstStageIntVars(),
                GRB_CONTINUOUS);

    d_xVars = d_model.addVars(d_problem.d_firstStageLowerBound.memptr(),
                              d_problem.d_firstStageUpperBound.memptr(),
                              d_problem.d_firstStageCoeffs.memptr(),
                              vTypes,
                              nullptr,
                              Amat.n_rows);

    // constraints
    GRBLinExpr lhs[Amat.n_cols];
    for (auto iter = Amat.begin(); iter != Amat.end(); ++iter)
        lhs[iter.col()] += *iter * d_xVars[iter.row()];

    char senses[Amat.n_cols];
    std::fill(senses,
              senses + d_problem.d_nFirstStageLeqConstraints,
              GRB_LESS_EQUAL);
    std::fill(senses + d_problem.d_nFirstStageLeqConstraints,
              senses + d_problem.d_nFirstStageLeqConstraints
                  + d_problem.d_nFirstStageGeqConstraints,
              GRB_GREATER_EQUAL);
    std::fill(senses + d_problem.d_nFirstStageLeqConstraints
                  + d_problem.d_nFirstStageGeqConstraints,
              senses + Amat.n_cols,
              GRB_EQUAL);

    GRBConstr *constrs = d_model.addConstrs(lhs,
                                            senses,
                                            d_problem.d_firstStageRhs.memptr(),
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

    // variable types
    char vTypes2[Wmat.n_rows];
    std::fill_n(vTypes2, d_problem.nSecondStageIntVars(), GRB_INTEGER);
    std::fill_n(vTypes2 + d_problem.nSecondStageIntVars(),
                Wmat.n_rows - d_problem.nSecondStageIntVars(),
                GRB_CONTINUOUS);

    // constraint senses
    char senses2[Tmat.n_cols];
    std::fill(senses2,
              senses2 + d_problem.d_nSecondStageLeqConstraints,
              GRB_LESS_EQUAL);
    std::fill(senses2 + d_problem.d_nSecondStageLeqConstraints,
              senses2 + d_problem.d_nSecondStageLeqConstraints
                  + d_problem.d_nSecondStageGeqConstraints,
              GRB_GREATER_EQUAL);
    std::fill(senses2 + d_problem.d_nSecondStageLeqConstraints
                  + d_problem.d_nSecondStageGeqConstraints,
              senses2 + Tmat.n_cols,
              GRB_EQUAL);

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        double const prob = d_problem.d_scenarioProbabilities[scenario];
        arma::vec const costs = prob * d_problem.d_secondStageCoeffs;

        // add variables
        GRBVar *yVars = d_model
                            .addVars(d_problem.d_secondStageLowerBound.memptr(),
                                     d_problem.d_secondStageUpperBound.memptr(),
                                     costs.memptr(),
                                     vTypes2,
                                     nullptr,
                                     Wmat.n_rows);

        // lhs expression of second-stage constraints, including Wy
        for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
            Tx[iter.col()] += *iter * yVars[iter.row()];

        auto const omega = d_problem.scenarios().colptr(scenario);
        GRBConstr *constrs = d_model.addConstrs(Tx,
                                                senses2,
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
