#include "ald.h"

void Ald::update(double *rhs)
{
  GRBsetdblattrarray(d_model, "RHS", 0, d_m2, rhs);  
}

