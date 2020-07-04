#ifndef MASTERPROBLEM_H
#define MASTERPROBLEM_H

#include "decompositions/decomposition.h"
#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>
#include <memory>

class MasterProblem
{
    Problem &d_problem;
    GRBModel d_model;

public:
    MasterProblem(GRBEnv &env, Problem &problem);

    /**
     * Adds decomposition <code>theta >= beta^T x + gamma</code>.
     */
    void addCut(Decomposition::Cut &cut);

    /**
     * Solves the master problem using the decomposition strategy. The master
     * problem is solved s.t. the optimality gap is smaller than tol.
     *
     * TODO STATISTICS - number of cuts, run-time?
     *
     * @return The (near) optimal first-stage decisions.
     */
    std::unique_ptr<arma::vec> solveWith(Decomposition &decomposition,
                                         double tol = 1e-4);

    /**
     * Returns the (exact) first stage objective value, that is, the value of
     * <code>c^T x*</code>.
     */
    double firstStageObjective() const;

    /**
     * Returns the approximation of the expected costs of the second stage
     * problems, that is, the value of <code>theta</code> in the master problem.
     */
    double secondStageObjective() const;

    /**
     * Returns the objective value of the master problem, that is, the sum
     * of the first and second stage objectives.
     */
    double objective() const
    {
        return firstStageObjective() + secondStageObjective();
    }
};

#endif
