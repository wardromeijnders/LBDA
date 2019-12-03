#include "deqform.h"

void DeqForm::solve(double time_limit)
{
  d_model.set(GRB_DoubleParam_TimeLimit, time_limit);
  d_model.optimize();
  
  int status = d_model.get(GRB_IntAttr_Status);
  
  if (status == 3)
    d_status = 2;
  else
  {
    d_status = 0;
   
    if (d_MIP)    
      d_MIPGap = d_model.get(GRB_DoubleAttr_MIPGap);
    d_objVal = d_model.get(GRB_DoubleAttr_ObjVal);
    d_objBound = d_model.get(GRB_DoubleAttr_ObjBound);        
    d_xVals = d_model.get(GRB_DoubleAttr_X, d_xVars, d_n1);     
    d_runTime = d_model.get(GRB_DoubleAttr_Runtime);
  }  
  
}