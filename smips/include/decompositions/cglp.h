#ifndef CGLP_H
#define CGLP_H

#include "master.h"

#include <gurobi_c++.h>

class Cglp
{
    /*
     * cuts generated by the cglp are of the form
     * d_pi_theta theta + d_pi_x * x >= d_pi_0
     * in Cglp::solve(), their corresponding values are stored in
     * pi_theta, pi_x, and pi_0, respectively
     */

public:
    size_t d_n1;

    GRBModel d_model;

    GRBVar d_lambda;  // corresponds to the last row
    GRBVar d_pi_theta;
    GRBVar *d_pi_x;
    GRBVar d_pi_0;

    std::vector<GRBConstr> d_constrs_theta;
    std::vector<GRBConstr *> d_constrs_x;
    std::vector<GRBConstr> d_constrs_cons;

    Cglp(GRBEnv &env, Master &master);
    ~Cglp();

    void add_disjunction(
        double *x, double theta, double *beta, double tau, double gamma);

    // changes the last row of the constraint matrix of d_model
    void chg_row(double *x_coefs, double cons);

    void solve(double &pi_theta, double *pi_x, double &pi_0);
};

#endif