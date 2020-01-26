#ifndef LAGRANGIAN_H
#define LAGRANGIAN_H

#include "problem.h"

#include <gurobi_c++.h>


class Lagrangian
{
    size_t d_m2;
    size_t d_n1;
    size_t d_n2;

    GRBModel d_model;
    GRBConstr *d_constrs;
    GRBVar *d_z_vars;

public:
    Lagrangian(GRBEnv &env, Problem &problem);

    Lagrangian(const Lagrangian &other);

    ~Lagrangian();

    void update(double *rhs, double *pi);

    double solve();
};

#endif
