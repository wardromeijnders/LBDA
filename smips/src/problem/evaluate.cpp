#include "problem.h"

double Problem::evaluate(arma::vec const &x)
{
    // computing cx
    double cx = arma::dot(d_firstStageCoeffs, x);
    arma::vec Tx = d_Tmat * x;

    // computing Q(x)
    if (not d_isSubProblemInitialised)
        initSub();  // initialize subproblem, rhs = 0.0

    double Q = 0.0;

    for (size_t scenario = 0; scenario != nScenarios(); ++scenario)
    {
        arma::vec rhs = scenarios().col(scenario) - Tx;

        d_sub.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), d_Wmat.n_cols);
        d_sub.optimize();

        Q += d_scenarioProbabilities[scenario]
             * d_sub.get(GRB_DoubleAttr_ObjVal);
    }

    return cx + Q;
}
