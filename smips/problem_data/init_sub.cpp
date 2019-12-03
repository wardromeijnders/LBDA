#include "problem.h"

void Problem::init_sub()
{
  char vTypes[d_n2];
  fill(vTypes, vTypes + d_p2, GRB_INTEGER);  
  fill(vTypes + d_p2, vTypes + d_n2, GRB_CONTINUOUS);    
      // cost vector
  double *qPtr = d_q.data();        // transform cost vector and omega to c-style array 
      // add variables
  GRBVar *vars = d_sub.addVars(d_l2.data(), d_u2.data(), qPtr, vTypes, NULL, d_n2); 
  
      // constraint senses
  char senses[d_m2];
  fill(senses,                       senses + d_ss_leq,            GRB_LESS_EQUAL);
  fill(senses + d_ss_leq,            senses + d_ss_leq + d_ss_geq, GRB_GREATER_EQUAL);
  fill(senses + d_ss_leq + d_ss_geq, senses + d_m2,                GRB_EQUAL);

      // constraint rhs
  double rhs[d_m2];
  fill(rhs, rhs + d_m2, 0.0);    
  
  GRBLinExpr Wy[d_m2];
  for (size_t conIdx = 0; conIdx != d_m2; ++conIdx)
  {
    double *row = d_Wmat[conIdx].data();      
    Wy[conIdx].addTerms(row, vars, d_n2);
  }
      // add constraints
  d_constrs = d_sub.addConstrs(Wy, senses, rhs, NULL, d_m2);
  d_sub_initialized = true;
  
  delete[] vars;
}