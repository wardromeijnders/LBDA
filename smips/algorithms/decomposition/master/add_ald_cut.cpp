#include "master.h"

bool Master::add_ald_cut(double *beta, double gamma, double tau, double *x, double theta, double tol)
{
  double betaxgamma = gamma;
  for (size_t var = 0; var != d_n1; ++var)
    betaxgamma += beta[var] * x[var];
  cout << "x = " << *x << '\n';
  cout << "theta = " << theta << '\n';
  cout << "outer Q(x) = " << betaxgamma << '\n';      
  
  if (betaxgamma <= theta + tol)
    return true;
  
  
  double bigM = 1e4;  
  /*
  GRBVar eta = d_model.addVar(0.0, 1e20, 0.0, GRB_CONTINUOUS, "eta");

      // binary vars to enforce eta <= ||x - x_bar||
  GRBVar *y_plus = d_model.addVars(d_n1, GRB_BINARY);
  GRBVar *y_minus = d_model.addVars(d_n1, GRB_BINARY);

      // add constraints forcing eta
  for (size_t var = 0; var != d_n1; ++var)
  {
    d_model.addConstr(eta - d_xVars[var] - bigM * y_plus[var], GRB_LESS_EQUAL, -x[var]);       // eta <= x - x_bar + My
    d_model.addConstr(eta + d_xVars[var] - bigM * y_minus[var], GRB_LESS_EQUAL, x[var]);       // eta <= x_bar - x + My 
  }
      // binary variables should sum to 2*d_n1 - 1, so that precisely one of the above constraints is binding,
      // since eta is chosen as large as possible by the solver, eta = ||x - x_bar||
  double ones[d_n1];
  fill_n(ones, d_n1, 1);
  GRBLinExpr y_sum;
  y_sum.addTerms(ones, y_plus, d_n1);
  y_sum.addTerms(ones, y_minus, d_n1);
  d_model.addConstr(y_sum, GRB_EQUAL, 2*d_n1 - 1);
      // add actual ald
  GRBLinExpr lhs = GRBLinExpr(d_theta); 
  GRBLinExpr rhs = gamma + tau * eta;
  rhs.addTerms(beta, d_xVars, d_n1);
  d_model.addConstr(lhs, GRB_GREATER_EQUAL, rhs);
        // clean up  
  delete[] y_plus;
  delete[] y_minus;
  */
  
  // c-api:
  GRBaddvar(d_cmodel, 0, NULL, NULL, 0.0, 0.0, 1e20, GRB_CONTINUOUS, NULL);     // eta
      // retrieve variable index  
  GRBupdatemodel(d_cmodel);
  int numVars;
  GRBgetintattr(d_cmodel, "NumVars", &numVars);
      // adding binary variables
  char types[2 * d_n1];
  fill_n(types, 2 * d_n1, GRB_BINARY);
  GRBaddvars(d_cmodel, 2 * d_n1, 0, NULL, NULL, NULL, NULL, NULL, NULL, types, NULL);  // binary yvars (first y+ then y-)
  
  for (size_t var = 0; var != d_n1; ++var)
  {
    int cind[3];
    cind[0] = numVars - 1; // refers to eta
    cind[1] = var + 1;     // refers to the var-th x-variable (theta precedes)
    cind[2] = numVars + var;  // refers to y+[var]
    double cval[] = {1, -1, -bigM};
    GRBaddconstr(d_cmodel, 3, cind, cval, GRB_LESS_EQUAL, -x[var], NULL);
  
    cind[2] = numVars + d_n1 + var;   // refers to y-[var]
    cval[1] = 1;    
    GRBaddconstr(d_cmodel, 3, cind, cval, GRB_LESS_EQUAL, x[var], NULL);
  }
      // force sum(y) = 2*d_n1 - 1
  int cind[2 * d_n1];  iota(cind, cind + 2 * d_n1, numVars);
  double cval[2 * d_n1];  fill_n(cval, 2 * d_n1, 1);
  GRBaddconstr(d_cmodel, 2 * d_n1, cind, cval, GRB_EQUAL, 2 * d_n1 - 1, NULL);
      // add actual ald cut
  int ald_cind[d_n1 + 2];
  iota(ald_cind, ald_cind + d_n1 + 1, 0);
  ald_cind[d_n1 + 1] = numVars - 1;
  
  double ald_cval[d_n1 + 2];
  ald_cval[0] = 1;
  for (size_t var = 0; var != d_n1; ++var)
    ald_cval[var + 1] = -beta[var];
  ald_cval[d_n1 + 1] = -tau;
  
  GRBaddconstr(d_cmodel, d_n1 + 2, ald_cind, ald_cval, GRB_GREATER_EQUAL, gamma, NULL);
  
  return false;
}