#include "deterministicequivalent.h"

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
    GRBLinExpr lhsExprs[Amat.n_cols];
    for (size_t conIdx = 0; conIdx != Amat.n_cols; ++conIdx)
        lhsExprs[conIdx].addTerms(Amat.colptr(conIdx), d_xVars, Amat.n_rows);

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

    GRBConstr *constrs = d_model.addConstrs(lhsExprs,
                                            senses,
                                            d_problem.d_firstStageRhs.memptr(),
                                            nullptr,
                                            Amat.n_cols);

    delete[] constrs;
}
