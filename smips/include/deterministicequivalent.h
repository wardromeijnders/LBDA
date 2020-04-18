#ifndef DETERMINISTICEQUIVALENT_H
#define DETERMINISTICEQUIVALENT_H

#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>
#include <memory>

class DeterministicEquivalent
{
    enum class status
    {
        SOLVED,
        UNSOLVED,
        INFEASIBLE
    };

    Problem const &d_problem;
    GRBModel d_model;
    status d_status;
    GRBVar *d_xVars;

    void initFirstStage();

    void initSecondStage();

public:
    DeterministicEquivalent(GRBEnv &env, Problem const &problem);

    ~DeterministicEquivalent();

    std::unique_ptr<arma::vec> solve(double timeLimit = 1e20);
};

#endif
