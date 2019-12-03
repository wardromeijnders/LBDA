#include "sub.h"

Sub::Multipliers Sub::solve()
{
  d_model.optimize();
  
      // computing shadow prices of upper bounds of y variables
  double *pi_u;
  if (d_model.get(GRB_IntAttr_Status) == 3)    // infeasible model (no complete recourse), dirty fix, we simply ignore the part 
    pi_u = new double[d_n2]();         // of the dual objective corresponding to the upper bounds (so we underestimate QLP)
  else
  {
    int *vbasis = d_model.get(GRB_IntAttr_VBasis, d_vars, d_n2);      // basis information
    pi_u = d_model.get(GRB_DoubleAttr_SAObjUp, d_vars, d_n2);    // largest value of obj coef for which current basis remains optimal
    for (size_t var = 0; var != d_n2; ++var)
    {
      if (vbasis[var] != -2)       // if the variable is not at the upper bound, then the shadow price is zero
        pi_u[var] = 0;
      else
        pi_u[var] = d_q[var] - pi_u[var];     // else: equal to the reduced costs
    }  
    delete[] vbasis;
  }
  
  return Multipliers {d_model.get(GRB_DoubleAttr_Pi, d_constrs, d_m2), pi_u};
}