#ifndef DETERMINISTICEQUIVALENT_H
#define DETERMINISTICEQUIVALENT_H

#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>
#include <memory>

class DeterministicEquivalent
{
    Problem const &d_problem;
    GRBModel d_model;

    void initFirstStage();

    void initSecondStage();

public:
    DeterministicEquivalent(GRBEnv &env, Problem const &problem);

    std::unique_ptr<arma::vec> solve(double timeLimit = 1e20);

    double objective() const
    {
        return d_model.get(GRB_DoubleAttr_ObjVal);
    }
};

#endif
