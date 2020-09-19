#ifndef STRONGBENDERS_H
#define STRONGBENDERS_H

#include "cutfamily.h"
#include "problemdata.h"

#include <armadillo>
#include <gurobi_c++.h>

class StrongBenders : public CutFamily
{
    GRBConstr *d_constrs;
    GRBVar *d_zVars;

    void update(arma::vec &rhs, arma::vec &pi);

public:
    StrongBenders(ProblemData const &problem);

    ~StrongBenders() override;

    CutFamily::Cut computeCut(arma::vec const &x) override;
};

#endif  // STRONGBENDERS_H
