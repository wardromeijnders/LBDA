#include "lagrangian.h"

void Lagrangian::update(double *rhs, double *pi)
{
  d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs, d_m2);
  d_model.set(GRB_DoubleAttr_Obj, d_z_vars, pi, d_n1);
}