#ifndef DEQFORM_H
#define DEQFORM_H

#include "problem.h"

#include <gurobi_c++.h>
#include <iosfwd>

class DeqForm
{
    enum class status
    {
        SOLVED,
        UNSOLVED,
        INFEASIBLE
    };

    size_t d_n1;
    GRBModel d_model;

    status d_status;

    double d_objVal = 0;  // TODO use these?
    double d_objBound = 0;
    double d_MIPGap = 0;
    double d_runTime = 0;

    bool d_isMip;

    void initFirstStage(size_t n1,
                        size_t p1,
                        size_t m1,
                        size_t fs_leq,
                        size_t fs_geq,
                        double *lb,
                        double *ub,
                        double *c,
                        double *rhs,
                        std::vector<std::vector<double>> &Amat);

    void initSecondStage(size_t n1,
                         size_t n2,
                         size_t p2,
                         size_t m2,
                         size_t S,
                         size_t ss_leq,
                         size_t ss_geq,
                         double *lb,
                         double *ub,
                         double *probs,
                         double *q,
                         std::vector<std::vector<double>> &Tmat,
                         std::vector<std::vector<double>> &Wmat,
                         std::vector<std::vector<double>> &omega);

public:
    GRBVar *d_xVars;  // TODO make private
    double *d_xVals;  // TODO make private

    DeqForm(GRBEnv &env, Problem &problem);

    ~DeqForm();

    void solve(double time_limit = 1e20);
};

#endif