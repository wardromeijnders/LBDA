#ifndef DETERMINISTICEQUIVALENT_H
#define DETERMINISTICEQUIVALENT_H

#include "problemdata.h"

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
    ProblemData const &d_problem;

    GRBEnv d_env = GRBEnv();
    GRBModel d_model;

    void initFirstStage();

    void initSecondStage();

public:
    DeterministicEquivalent(ProblemData const &problem);

    /**
     * Solves the deterministic equivalent.
     *
     * @param timeLimit Time limit for the Gurobi solver, in seconds.
     * @return Vector of optimal first-stage decisions, or best so far.
     */
    std::unique_ptr<arma::vec> solve(double timeLimit = arma::datum::inf);

    /**
     * @return True if the model was solved to optimality, false otherwise.
     */
    bool isOptimal() const;

    /**
     * @return Returns the optimality gap of the solution returned by
     *         <code>solve</code>.
     */
    double mipGap() const;

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
