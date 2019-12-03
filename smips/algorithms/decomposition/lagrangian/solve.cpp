#include "lagrangian.h"

double Lagrangian::solve()
{
  d_model.optimize();
  
  return d_model.get(GRB_DoubleAttr_ObjVal);
}