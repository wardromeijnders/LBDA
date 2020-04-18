#include "deterministicequivalent.h"

void DeterministicEquivalent::initSecondStage()
{
    auto const &Tmat = d_problem.Tmat();
    auto const &Wmat = d_problem.Wmat();

    GRBLinExpr Tx[Tmat.n_cols];
    for (size_t conIdx = 0; conIdx != Tmat.n_cols; ++conIdx)
        Tx[conIdx].addTerms(Tmat.colptr(conIdx), d_xVars, Tmat.n_rows);

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
        // TODO: I've removed a duplication for TxWy here, which did not compile
        for (size_t conIdx = 0; conIdx != Wmat.n_cols; ++conIdx)
            Tx[conIdx].addTerms(Wmat.colptr(conIdx), yVars, Wmat.n_rows);

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