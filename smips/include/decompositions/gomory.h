#ifndef GOMORY_H
#define GOMORY_H

#include "problem.h"
#include "relaxation.h"

#include <gurobi_c++.h>


class Gomory : public Relaxation
{
    size_t d_m2;
    size_t d_n2;
    size_t d_ss_leq;
    size_t d_ss_geq;

    double const *d_l2;
    double const *d_u2;

    GRBConstr *d_constrs;
    GRBVar *d_vars;

public:
    Gomory(GRBEnv &env, Problem const &problem);

    Gomory(Gomory const &other);

    ~Gomory() override;

    void update(double *rhs, int const *vBasis, int const *cBasis);

    void setTimeLimit(double timeLimit);
};

#endif
