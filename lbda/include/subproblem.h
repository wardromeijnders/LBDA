#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "problemdata.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>

/**
 * Second-stage (sub) problem of the two-stage decomposition.
 */
class SubProblem
{
    GRBEnv d_env = GRBEnv();

    GRBModel d_model;
    GRBConstr *d_constrs;
    GRBVar *d_vars;

    ProblemData const &d_problem;

public:
    struct BasisInfo
    {
        arma::Col<int> vBasis;
        arma::Col<int> cBasis;
    };

    struct Duals
    {
        arma::vec lambda;
        arma::vec pi_u;
    };

    SubProblem(ProblemData const &problem);

    ~SubProblem();

    BasisInfo const basisInfo();

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
