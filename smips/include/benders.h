#ifndef BENDERS_H
#define BENDERS_H

#include "decompositions/cglp.h"
#include "decompositions/gomory.h"
#include "decompositions/lagrangian.h"
#include "master.h"
#include "problem.h"
#include "sub.h"


class Benders
{
    size_t d_n1;
    size_t d_n2;
    size_t d_m2;
    size_t d_S;

    size_t d_nCuts;
    double d_runTime;

    GRBEnv &d_env;

    Problem &d_problem;  // contains problem data
    Master d_master;     // master problem
    Sub d_sub;           // sub-problem
    Lagrangian d_lr;     // lagrangian relaxation
    Gomory d_gomory;     // Gomory relaxation

    // for each scenario, we store the basis matrices that we
    // have visited (encoded by vBasis, cBasis)
    std::vector<std::vector<std::vector<double>>> d_visited;

    // for each visited basis matrix, we store the
    // corresponding gomory objective value
    std::vector<std::vector<double>> d_objectives;

    double computeGomory(size_t s,
                         int *vBasis,
                         int *cBasis,
                         double const *ws,
                         double const *alpha);

    void computeTx(double const *x, double *Tx);

    void lbdaCut(double *x, double *alpha, double *beta, double &gamma);

    void lpCut(double *x, double *beta, double &gamma);

    void sbCut(double *x, double *beta, double &gamma);

public:
    double *d_xvals;  // TODO make private

    Benders(GRBEnv &env, GRBenv *c_env, Problem &problem);

    Benders(const Benders &other);

    ~Benders();

    void lpSolve(double tol = 1e-4);

    void strongBenders(double tol = 1e-4);

    void lbda(double *alpha, double timeLimit = 1e6, double tol = 1e-4);
};

#endif
