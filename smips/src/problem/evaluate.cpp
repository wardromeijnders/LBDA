#include "problem.h"

double Problem::evaluate(arma::vec const &x)
{
    // computing cx
    double cx = arma::dot(d_c, x);
    arma::vec Tx = d_Tmat * x;

    // computing Q(x)
    if (not d_sub_initialized)
        initSub();  // initialize subproblem, rhs = 0.0

    double Q = 0.0;

    for (size_t s = 0; s != d_S; ++s)
    {
        arma::vec rhs = d_omega.col(s) - Tx;

        d_sub.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), d_m2);
        d_sub.optimize();

        Q += d_probs[s] * d_sub.get(GRB_DoubleAttr_ObjVal);
    }

    return cx + Q;
}
