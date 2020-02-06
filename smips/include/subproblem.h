#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>

class SubProblem
{
    GRBModel d_model;
    GRBConstr *d_constrs;
    GRBVar *d_vars;

    Problem const &d_problem;

public:
    SubProblem(GRBEnv &env, Problem const &problem);

    SubProblem(SubProblem const &other);

    ~SubProblem();

    void update(arma::vec &rhs);  // 1

    void update(arma::vec &&rhs);  // 2

    void solve();

    struct GomInfo  // TODO maybe combine with Multipliers?
    {
        arma::vec lambda;
        arma::Col<int> vBasis;
        arma::Col<int> cBasis;
    };

    struct Multipliers
    {
        arma::vec lambda;
        arma::vec pi_u;
    };

    Multipliers const multipliers();

    GomInfo const gomInfo();
};

#endif
