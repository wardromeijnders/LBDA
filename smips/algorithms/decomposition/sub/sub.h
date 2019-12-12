#ifndef SUB_H
#define SUB_H

#include "../../../problem_data/problem.h"
#include "gurobi_c++.h"

using namespace std;

class Sub
{
public:
    GRBModel d_model;
    size_t d_m2, d_n2;
    GRBConstr *d_constrs;
    GRBVar *d_vars;
    vector<double> &d_q;

    Sub(GRBEnv &env, Problem &problem);
    Sub(const Sub &other);
    ~Sub();

    void update(double *rhs);

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


    Multipliers solve();
    GomInfo solve2();
};

#endif
