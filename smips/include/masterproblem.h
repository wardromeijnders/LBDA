#ifndef MASTERPROBLEM_H
#define MASTERPROBLEM_H

#include "cuts/cut.h"
#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>
#include <memory>

class MasterProblem
{
    // TODO is this useful?
    // GRBVar *d_xVars;
    // GRBVar d_theta;
    // GRBModel d_model;
    GRBmodel *d_cmodel;

    // internal storage: only valid for regular l-shaped and lbda cuts
    std::vector<std::vector<double>> d_xCoeffs;
    std::vector<double> d_cuts;

    // slack variable identities slack = kappa * theta - beta * x - gamma
    // TODO is this useful?
    // std::vector<double> d_kappa;
    // std::vector<std::vector<double>> d_beta;
    // std::vector<double> d_gamma;

    size_t d_n1;  // TODO make this reference problem?
    size_t d_nSlacks;

public:
    struct Solution
    {
        std::unique_ptr<arma::vec> x;
        double theta;
    };

    MasterProblem(GRBEnv &env, GRBenv *c_env, Problem &problem);

    MasterProblem(MasterProblem const &other);

    ~MasterProblem();

    /**
     * Determines if the proposed cut is violated by the current solution.
     */
    static bool isValidCut(Cut::CutResult const &cutResult,
                           MasterProblem::Solution const &sol,
                           double tol);

    /**
     * Adds cut <code>theta >= beta^T x + gamma</code>.
     */
    void addCut(Cut::CutResult &cutResult);

    [[nodiscard]] std::vector<double> const &cuts() const;

    [[nodiscard]] std::vector<std::vector<double>> const &xCoeffs() const;

    [[nodiscard]] size_t n1() const;

    Solution const solve();
};

#endif
