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
    struct GomInfo
    {
        arma::vec lambda;
        arma::Col<int> vBasis;
        arma::Col<int> cBasis;
    };

    struct Duals
    {
        arma::vec lambda;
        arma::vec pi_u;
    };

    SubProblem(GRBEnv &env, Problem const &problem);

    SubProblem(SubProblem const &other);

    ~SubProblem();

    GomInfo const gomInfo();

    Duals const duals();

    void updateRhs(arma::vec &rhs);

    void updateRhs(arma::vec &&rhs);

    void solve();

    /**
     * Returns the objective value of the sub problem.
     */
    double objective() const
    {
        return d_model.get(GRB_DoubleAttr_ObjVal);
    }
};

#endif
