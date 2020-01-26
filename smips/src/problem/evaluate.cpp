#include "problem.h"

double Problem::evaluate(double const *x)
{
    // computing cx
    double cx = 0.0;
    for (size_t var = 0; var != d_n1; ++var)
        cx += d_c[var] * x[var];

    // computing z = Tx
    double Tx[d_m2];
    for (size_t zvar = 0; zvar != d_m2; ++zvar)
    {
        Tx[zvar] = 0.0;
        for (size_t xvar = 0; xvar != d_n1; ++xvar)
            Tx[zvar] += d_Tmat[zvar][xvar] * x[xvar];
    }

    // computing Q(x)
    if (not d_sub_initialized)
        initSub();  // initialize subproblem, rhs = 0.0

    double Q = 0.0;

    for (size_t s = 0; s != d_S; ++s)
    {
        double *ws = d_omega[s].data();
        double rhs[d_m2];

        for (size_t zvar = 0; zvar != d_m2; ++zvar)
            rhs[zvar] = ws[zvar] - Tx[zvar];

        d_sub.set(GRB_DoubleAttr_RHS, d_constrs, rhs, d_m2);
        d_sub.optimize();

        Q += d_probs[s] * d_sub.get(GRB_DoubleAttr_ObjVal);
    }

    return cx + Q;
}
