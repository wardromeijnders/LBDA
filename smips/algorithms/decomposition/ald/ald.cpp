#include "ald.h"

Ald::Ald(GRBenv *env, Problem &problem)
:
d_Tmat(problem.d_Tmat)
{
  GRBnewmodel(env, &d_model, NULL, 0, NULL, NULL, NULL, NULL, NULL);  
  
      // Gomory cuts are derived from the simplex tableau, which is easiest to construct in a problem 
      // with only equality constraints (given the possibilities of the Gurobi c-api) (an alternative is
      // to investigate precisely how gurobi handles inequality constraints) 
  size_t n2 = problem.d_n2; 
  size_t m2 = problem.d_m2;
  size_t nLeq = problem.d_ss_leq; 
  size_t nGeq = problem.d_ss_geq;
  int nSlacks = nLeq + nGeq;
  
  d_n1 = problem.d_n1; 
  d_n2 = n2 + nSlacks;       // number of second-stage variables (including slacks)
  d_m2 = m2; 
  d_p2 = problem.d_p2;
  
  
  // adding variables  (assumed continuous, lower and upper bounds ignored)  
  GRBaddvars(d_model, n2, 0, NULL, NULL, NULL, problem.d_q.data(), NULL, NULL, NULL, NULL);
  
  // adding constraints
  int cind[n2];
  iota(cind, cind + n2, 0);
  
  for (size_t con = 0; con != m2; ++con)    
    GRBaddconstr(d_model, n2, cind, problem.d_Wmat[con].data(), GRB_EQUAL, 0, NULL);
  // adding slacks    
  int vbeg[nSlacks];
  iota(vbeg, vbeg + nSlacks, 0);
  int *vind = vbeg;  
  double vval[nSlacks];
  fill_n(vval, nLeq, 1);
  fill_n(vval + nLeq, nGeq, -1);
  
  GRBaddvars(d_model, nSlacks, nSlacks, vbeg, vind, vval, NULL, NULL, NULL, NULL, NULL);
  GRBupdatemodel(d_model); 
  

}