#ifndef GOMORY_H
#define GOMORY_H

#include "problem.h"

#include <gurobi_c++.h>


class Gomory
{
    size_t d_m2;
    size_t d_n2;
    size_t d_ss_leq;
    size_t d_ss_geq;

    double *d_l2;
    double *d_u2;

    GRBModel d_model;
    GRBConstr *d_constrs;
    GRBVar *d_vars;

public:
    Gomory(GRBEnv &env, Problem &problem);

    Gomory(const Gomory &other);

    ~Gomory();

    void update(double *rhs, int *vBasis, int *cBasis);

    double solve();

    void setTimeLimit(double timeLimit);
};

#endif
