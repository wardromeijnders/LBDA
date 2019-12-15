#ifndef MASTER_H
#define MASTER_H

#include "problem.h"

#include <gurobi_c++.h>
#include <gurobi_c.h>
#include <iostream>
#include <vector>


class Master
{
public:
    // GRBVar *d_xVars;
    // GRBVar d_theta;
    // GRBModel d_model;
    GRBmodel *d_cmodel;

    // internal storage: only valid for regular l-shaped and lbda cuts
    std::vector<std::vector<double>> d_xcoefs;
    std::vector<double> d_cons;

    // slack variable identities s = kappa * theta - beta * x - gamma
    std::vector<double> d_kappa;
    std::vector<std::vector<double>> d_beta;
    std::vector<double> d_gamma;

    size_t d_n1;
    size_t d_nSlacks;


    // storing the optimality cut coefficients
    // vector<vector<double>> d_xcoefs;

    Master(GRBEnv &env,
           GRBenv *c_env,
           Problem &problem);  // initializes d_model and its variables
    Master(const Master &other);
    ~Master();  // deletes vars and frees d_cmodel

    // adds cut theta >= beta^T x + gamma, if this cut is violated (ret = true),
    // else cut is not added (ret = false).
    bool addCut(double *beta, double gamma, double *x, double theta, double tol);
    bool add_ald_cut(double *beta,
                     double gamma,
                     double tau,
                     double *x,
                     double theta,
                     double tol);
    bool add_zk_cut(double *beta,
                    double gamma,
                    double kappa,
                    double *x,
                    double theta,
                    double tol);  // adds the cut kappa theta >= beta^T (x,s) +
                                  // gamma (this cuts also features slacks)

    struct Solution
    {
        double *xVals;
        double thetaVal;
    };

    Solution solve();  // solves the gurobimodel
};

#endif
