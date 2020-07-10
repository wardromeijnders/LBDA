#ifndef CUT_H
#define CUT_H

#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>


class CutFamily
{
protected:
    GRBEnv &d_env;
    GRBModel d_model;
    Problem const &d_problem;

    explicit CutFamily(GRBEnv &env, Problem const &problem);

public:
    struct Cut
    {
        arma::vec beta;
        double gamma;
    };

    virtual CutFamily::Cut computeCut(arma::vec const &x) = 0;

    virtual double solve();
};

#endif  // CUT_H
