#ifndef SUB_H
#define SUB_H

#include "problem.h"

#include <gurobi_c++.h>
#include <iosfwd>

class Sub
{
    size_t d_m2;
    size_t d_n2;

    std::vector<double> &d_q;

    GRBModel d_model;
    GRBConstr *d_constrs;
    GRBVar *d_vars;

    struct GomInfo
    {
        double *lambda;
        int *vBasis;
        int *cBasis;
    };

    struct Multipliers
    {
        double *lambda;
        double *pi_u;
    };

public:
    Sub(GRBEnv &env, Problem &problem);

    Sub(Sub const &other);

    ~Sub();

    void update(double *rhs);

    void solve();

    Multipliers const multipliers();

    GomInfo const gomInfo();
};

#endif
