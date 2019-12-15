#include "sub.h"

Sub::Multipliers Sub::solve()
{
    d_model.optimize();

    // computing shadow prices of upper bounds of y variables
    double *pi_u;

    // No complete recourse. Dirty fix: ignore the part of the dual objective
    // corresponding to the upper bounds (so we underestimate QLP)
    if (d_model.get(GRB_IntAttr_Status) == GRB_INFEASIBLE)
        pi_u = new double[d_n2]();
    else
    {
        // basis information
        int *vbasis = d_model.get(GRB_IntAttr_VBasis, d_vars, d_n2);

        // largest value of obj coef for which current basis remains optimal
        pi_u = d_model.get(GRB_DoubleAttr_SAObjUp, d_vars, d_n2);

        for (size_t var = 0; var != d_n2; ++var)
        {
            // if the variable is not at the upper
            // bound, then the shadow price is zero
            if (vbasis[var] != -2)
                pi_u[var] = 0;
            else
                // else: equal to the reduced costs
                pi_u[var] = d_q[var] - pi_u[var];
        }

        delete[] vbasis;
    }

    return Multipliers{d_model.get(GRB_DoubleAttr_Pi, d_constrs, d_m2), pi_u};
}
