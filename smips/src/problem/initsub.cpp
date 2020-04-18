#include "problem.h"

void Problem::initSub()
{
    char vTypes[d_Wmat.n_rows];
    std::fill(vTypes, vTypes + nSecondStageIntVars(), GRB_INTEGER);
    std::fill(vTypes + nSecondStageIntVars(),
              vTypes + d_Wmat.n_rows,
              GRB_CONTINUOUS);

    GRBVar *vars = d_sub.addVars(d_secondStageLowerBound.memptr(),
                                 d_secondStageUpperBound.memptr(),
                                 d_secondStageCoeffs.memptr(),
                                 vTypes,
                                 nullptr,
                                 d_Wmat.n_rows);

    // constraint senses
    char senses[d_Wmat.n_cols];
    std::fill(senses, senses + d_nSecondStageLeqConstraints, GRB_LESS_EQUAL);
    std::fill(senses + d_nSecondStageLeqConstraints,
              senses + d_nSecondStageLeqConstraints
                  + d_nSecondStageGeqConstraints,
              GRB_GREATER_EQUAL);
    std::fill(senses + d_nSecondStageLeqConstraints
                  + d_nSecondStageGeqConstraints,
              senses + d_Wmat.n_cols,
              GRB_EQUAL);

    // constraint rhs
    double rhs[d_Wmat.n_cols];
    std::fill(rhs, rhs + d_Wmat.n_cols, 0.0);

    GRBLinExpr Wy[d_Wmat.n_cols];
    for (size_t conIdx = 0; conIdx != d_Wmat.n_cols; ++conIdx)
        Wy[conIdx].addTerms(d_Wmat.colptr(conIdx), vars, d_Wmat.n_rows);

    // add constraints
    d_constrs = d_sub.addConstrs(Wy, senses, rhs, nullptr, d_Wmat.n_cols);
    d_isSubProblemInitialised = true;

    delete[] vars;
}
