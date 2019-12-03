#include "gomory.h"

Gomory::Gomory(const Gomory &other)
:
  d_model(other.d_model),
  d_m2(other.d_m2), 
  d_n2(other.d_n2),
  d_ss_leq(other.d_ss_leq),
  d_ss_geq(other.d_ss_geq),
  d_l2(other.d_l2),
  d_u2(other.d_u2)  
{
  d_constrs = d_model.getConstrs();
  d_vars = d_model.getVars();
}