#ifndef LAGRANGIAN_H
#define LAGRANGIAN_H

#include "problem.h"
#include "relaxation.h"

#include <armadillo>
#include <gurobi_c++.h>


class Lagrangian : public Relaxation
{
    size_t d_m2;
    size_t d_n1;
    size_t d_n2;

    GRBConstr *d_constrs;
    GRBVar *d_z_vars;

public:
    Lagrangian(GRBEnv &env, Problem const &problem);

    Lagrangian(Lagrangian const &other);

    ~Lagrangian() override;

    void update(arma::vec &rhs, arma::vec &pi);
};

#endif
