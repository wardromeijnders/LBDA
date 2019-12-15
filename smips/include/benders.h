#ifndef BENDERS_H
#define BENDERS_H

#include "decompositions/ald.h"
#include "decompositions/cglp.h"
#include "decompositions/gomory.h"
#include "decompositions/lagrangian.h"
#include "decompositions/pslp.h"
#include "master.h"
#include "problem.h"
#include "sub.h"

#include <algorithm>
#include <chrono>
#include <memory>


class Benders
{
public:
    Problem &d_problem;  // contains problem data
    GRBEnv &d_env;
    size_t d_n1, d_m2, d_n2, d_S;
    Master d_master;  // master problem
    Sub d_sub;        // sub-problem
    Lagrangian d_lr;  // lagrangian relaxation
    Gomory d_gomory;  // Gomory relaxation
    Ald d_ald;        // For deriving ALD cuts
    Pslp d_pslp;      // For deriving (strong) ZK cuts

    size_t d_nCuts;
    double d_runTime;

    // for each scenario, we store the basis matrices that we
    // have visited (encoded by vBasis, cBasis)
    std::vector<std::vector<std::vector<double>>> d_visited;

    // for each visited basis matrix, we store the
    // corresponding gomory objective value
    std::vector<std::vector<double>> d_objectives;

    double *d_xvals;

    // initializes d_master and d_sub with both arguments
    Benders(GRBEnv &env, GRBenv *c_env, Problem &problem);
    Benders(const Benders &other);

    ~Benders();

    void computeTx(double *x, double *Tx);  // computes Tx (rba)

    // return gamma. beta is returned by argument
    double lpCut(double *x, double *beta);
    void sb_cut(double *x, double *beta, double &gamma);
    void ald_cut(
        double *x, double *beta, double &tau, double &gamma, size_t maxRounds);
    void zk_cut(double *x,
                double theta,
                double *beta,
                double &gamma,
                double &kappa,
                size_t maxRounds);
    double lbdaCut(double *x, double *beta, double *alpha);
    double compute_gomory(
        size_t s, int *vBasis, int *cBasis, double *ws, double *alpha);

    void lpSolve(double tol = 1e-4);
    void strong_benders(double tol = 1e-4);  // L-shaped algorithm
    void lbda(double *alpha,
              double gomoryTimeLimit = 1e6,
              double tol = 1e-4);  // LBDA(alpha)
    void ald_solve(double tol = 1e-4, size_t maxRounds = 25);
    void zk_solve(bool strong,
                  double tau_bar = 0,
                  double tol = 1e-4,
                  size_t maxRounds = 25);
    void ald_affine(double tol = 1e-4, size_t maxRounds = 25);
};

#endif
