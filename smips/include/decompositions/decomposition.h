#ifndef CUT_H
#define CUT_H

#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>


class Decomposition
{
protected:
    GRBEnv &d_env;
    GRBModel d_model;
    Problem const &d_problem;

    explicit Decomposition(GRBEnv &env, Problem const &problem);

public:
    struct Cut
    {
        arma::vec beta;
        double gamma;
    };

    virtual Decomposition::Cut computeCut(arma::vec const &x) = 0;

    virtual double solve();
};

#endif  // CUT_H
