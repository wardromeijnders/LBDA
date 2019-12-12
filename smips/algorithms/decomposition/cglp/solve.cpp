#include "cglp.h"

void Cglp::solve(double &pi_theta, double *pi_x, double &pi_0)
{
  d_model.optimize();
  cout << "objval: " << d_model.get(GRB_DoubleAttr_ObjVal) << '\n';
  pi_theta = d_pi_theta.get(GRB_DoubleAttr_X);
  
  double *pi_x_vals = d_model.get(GRB_DoubleAttr_X, d_pi_x, d_n1);
  copy(pi_x_vals, pi_x_vals + d_n1, pi_x);
  delete[] pi_x_vals;
  
  
  pi_0 = d_pi_0.get(GRB_DoubleAttr_X);  
}