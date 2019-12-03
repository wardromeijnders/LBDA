#include "gomory.h"

Gomory::Gomory(GRBEnv &env, Problem &problem)
:
d_model(env)
{
  size_t n2 = problem.d_n2;
  size_t m2 = problem.d_m2;
  size_t p2 = problem.d_p2;
  
  size_t ss_leq, ss_geq;
  ss_leq = problem.d_ss_leq; ss_geq = problem.d_ss_geq;
  
  d_n2 = n2; d_m2 = m2;
  d_ss_leq = ss_leq; d_ss_geq = ss_geq;
  
  d_l2 = problem.d_l2.data(); d_u2 = problem.d_u2.data();
  
         // variable types    
  char vTypes[n2];
  fill(vTypes, vTypes + p2, GRB_INTEGER);  
  fill(vTypes + p2, vTypes + n2, GRB_CONTINUOUS);    
      // cost vector
  double *q = problem.d_q.data();        // transform cost vector and omega to c-style array 
      // add variables
  d_vars = d_model.addVars(problem.d_l2.data(), problem.d_u2.data(), q, vTypes, NULL, n2); 
  
      // constraint senses
  char senses[m2];
  fill(senses,                   senses + ss_leq,          GRB_LESS_EQUAL);
  fill(senses + ss_leq,          senses + ss_leq + ss_geq, GRB_GREATER_EQUAL);
  fill(senses + ss_leq + ss_geq, senses + m2,              GRB_EQUAL);
      // constraint rhs
  double rhs[m2];
  fill(rhs, rhs + m2, 0.0);    
  
      // constraint lhs
  vector<vector<double>> &Wmat = problem.d_Wmat;
  
  GRBLinExpr Wy[m2];
  for (size_t conIdx = 0; conIdx != m2; ++conIdx)
  {
    double *row = Wmat[conIdx].data();      
    Wy[conIdx].addTerms(row, d_vars, n2);
  }
      // add constraints
  d_constrs = d_model.addConstrs(Wy, senses, rhs, NULL, m2);
  d_model.update();
}