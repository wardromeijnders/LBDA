#ifndef STRONGBENDERS_H
#define STRONGBENDERS_H

#include "cutfamily.h"
#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>

class StrongBenders : public CutFamily
{
    GRBConstr *d_constrs;
    GRBVar *d_xVars;

    void update(arma::vec &rhs, arma::vec &pi);

public:
    StrongBenders(GRBEnv &env, Problem const &problem);

    ~StrongBenders() override;

    CutFamily::Cut computeCut(arma::vec const &x) override;
};

#endif  // STRONGBENDERS_H
