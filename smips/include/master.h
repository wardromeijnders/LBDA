#ifndef MASTER_H
#define MASTER_H

#include "problem.h"

#include <gurobi_c++.h>
#include <iosfwd>

class Master
{
    struct Solution
    {
        double *xVals;
        double thetaVal;
    };

    // GRBVar *d_xVars;
    // GRBVar d_theta;
    // GRBModel d_model;
    GRBmodel *d_cmodel;

    // internal storage: only valid for regular l-shaped and lbda cuts
    std::vector<std::vector<double>> d_xCoeffs;
    std::vector<double> d_cuts;

    // slack variable identities slack = kappa * theta - beta * x - gamma
    std::vector<double> d_kappa;
    std::vector<std::vector<double>> d_beta;
    std::vector<double> d_gamma;

    size_t d_n1;
    size_t d_nSlacks;

    // storing the optimality cut coefficients
    // vector<vector<double>> d_xCoeffs;

public:
    Master(GRBEnv &env, GRBenv *c_env, Problem &problem);

    Master(Master const &other);

    ~Master();

    /**
     * Adds cut <code>theta >= beta^T x + gamma</code> if this cut is violated.
     * @return  Is the cut violated? If true, the cut was added; else not.
     */
    bool addCut(double *beta, double gamma, double *x, double theta, double tol);

    [[nodiscard]] std::vector<double> const &cuts() const;

    [[nodiscard]] std::vector<std::vector<double>> const &xCoeffs() const;

    [[nodiscard]] size_t n1() const;

    Solution const solve();
};

#endif
