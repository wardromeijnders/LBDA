#ifndef GOMORY_H
#define GOMORY_H

#include "gurobi_c++.h"

#include "../../../problem_data/problem.h"


using namespace std;

class Gomory
{
  public:
    GRBModel d_model;
    size_t d_m2, d_n2;
    size_t d_ss_leq, d_ss_geq;    
    GRBConstr *d_constrs;
    GRBVar *d_vars;
    double *d_l2, *d_u2;     
    Gomory(GRBEnv &env, Problem &problem);
    Gomory(const Gomory &other);
    ~Gomory();

    void update(double *rhs, int *vBasis, int *cBasis);
    double solve();
    void setTimeLimit(double timeLimit);
};

#endif
