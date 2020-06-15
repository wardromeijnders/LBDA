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
    /**
     * Reference to the problem instance. Cannot be declared const, as
     * Gurobi expects non-const pointers to some of the Problem data.
     */
    Problem &d_problem;

    /**
     * Gurobi model for the master problem.
     */
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
    std::unique_ptr<arma::vec> solve(Decomposition &decomposition,
                                     double tol = 1e-4);
};

#endif
