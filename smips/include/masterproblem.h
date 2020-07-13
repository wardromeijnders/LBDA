#ifndef MASTERPROBLEM_H
#define MASTERPROBLEM_H

#include "cutfamilies/cutfamily.h"
#include "problem.h"

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>
#include <memory>

/**
 * First-stage (master) problem of the two-stage decomposition.
 */
class MasterProblem
{
    Problem &d_problem;
    GRBModel d_model;

public:
    /**
     * Constructs the master problem from the data in the passed-in problem
     * instance.
     *
     * @param env           Gurobi environment.
     * @param problem       Problem instance.
     * @param lowerBound    Lower bound for theta, the approximation of the
     *                      expected cost-to-go. Default 0.
     * @param upperBound    Upper bound for theta. Default +inf.
     */
    MasterProblem(GRBEnv &env,
                  Problem &problem,
                  double lowerBound = 0.,
                  double upperBound = arma::datum::inf);

    /**
     * Adds cut <code>theta >= beta^T x + gamma</code>.
     */
    void addCut(CutFamily::Cut &cut);

    /**
     * Solves the master problem using the given cutting strategy. The
     * master problem is solved s.t. the optimality gap is smaller than tol.
     *
     * @param cutFamily The cutting strategy (family) to use.
     * @param tol       Maximum acceptable optimality gap.
     *
     * @return Vector of (near) optimal first-stage decisions.
     */
    std::unique_ptr<arma::vec> solveWith(CutFamily &cutFamily,
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
        return d_model.get(GRB_DoubleAttr_ObjVal);
    }
};

#endif
