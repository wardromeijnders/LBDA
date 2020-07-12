#ifndef DETERMINISTICEQUIVALENT_H
#define DETERMINISTICEQUIVALENT_H

#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>
#include <memory>

/**
 * Deterministic equivalent formulation of the two-stage problem. This is also
 * sometimes called the extensive form.
 */
class DeterministicEquivalent
{
    Problem const &d_problem;
    GRBModel d_model;

    void initFirstStage();

    void initSecondStage();

public:
    DeterministicEquivalent(GRBEnv &env, Problem const &problem);

    /**
     * Solves the deterministic equivalent.
     *
     * @param timeLimit Time limit for the Gurobi solver, in seconds.
     * @return Vector of optimal first-stage decisions.
     */
    std::unique_ptr<arma::vec> solve(double timeLimit = 1e20);

    /**
     * @return First-stage objective value.
     */
    double firstStageObjective();

    /*
     * @return expected cost-to-go of the second-stage.
     */
    double secondStageObjective();

    /**
     * @return Objective value.
     */
    double objective() const
    {
        return d_model.get(GRB_DoubleAttr_ObjVal);
    }
};

#endif
