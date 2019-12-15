#ifndef LAGRANGIAN_H
#define LAGRANGIAN_H

#include "problem.h"

#include <gurobi_c++.h>


class Lagrangian
{
public:
    GRBModel d_model;
    size_t d_n1, d_m2, d_n2;
    GRBConstr *d_constrs;
    GRBVar *d_z_vars;
    Lagrangian(GRBEnv &env, Problem &problem);
    Lagrangian(const Lagrangian &other);
    ~Lagrangian();

    void update(double *rhs, double *pi);
    double solve();
};

#endif