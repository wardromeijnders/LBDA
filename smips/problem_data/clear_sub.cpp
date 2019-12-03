#include "problem.h"

void Problem::clear_sub()
{
  if (not d_sub_initialized)      // if not already initialized
    return;                       // then do nothing 
  
  
        // else, clear variables and cosnstraints
  GRBConstr *constrs = d_sub.getConstrs();
  GRBVar *vars = d_sub.getVars();
  
  size_t nConstrs = d_sub.get(GRB_IntAttr_NumConstrs);
  size_t nVars = d_sub.get(GRB_IntAttr_NumVars);
  
  for (size_t idx = 0; idx != nConstrs; ++idx)
    d_sub.remove(constrs[idx]);
    
  for (size_t idx = 0; idx != nVars; ++idx)
    d_sub.remove(vars[idx]);
 
  delete[] constrs;               // heap allocated and no longer required 
  delete[] vars;
  
  delete[] d_constrs;             // heap allocated, but d_sub is cleared 
  d_sub_initialized = false;      // this ensures that: (1) init_sub() is called if evaluate() is called, and destructor does not call delete[] d_constrs
}