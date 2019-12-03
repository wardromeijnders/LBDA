#include "zk.h"

void ZK::update(double *x, double theta)      // updates rhs
{
  double rhs[d_nConstrs];
  
  for (size_t con = 0; con != d_nConstrs; ++con)
  {
    rhs[con] = d_omega[con] - d_tau[con] * theta;
    vector<double> &Trow = d_Tmat[con];
    for (size_t var = 0; var != Trow.size(); ++var)
      rhs[con] -= Trow[var] * x[var];
  } 
  
  GRBsetdblattrarray(d_model, "RHS", 0, d_nConstrs, rhs);


}