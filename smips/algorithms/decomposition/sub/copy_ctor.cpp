#include "sub.h"

Sub::Sub(const Sub &other):
d_n2(other.d_n2),
d_m2(other.d_m2),
d_model(other.d_model),
d_q(other.d_q)
{
  d_vars = d_model.getVars();
  d_constrs = d_model.getConstrs(); 
}