#include "benders.h"

void Benders::ald_cut(double *x, double *beta, double &tau, double &gamma, size_t maxRounds)
{
  vector<vector<double>> &omega = d_problem.d_omega;
  vector<vector<double>> &Tmat = d_problem.d_Tmat;
  vector<double> &probs = d_problem.d_probs;
  
  double Tx[d_m2];
  computeTx(x, Tx);      // Tx is rba

      // cut coefficients: initialize to zero  
  double dual[d_m2];
  fill(dual, dual + d_m2, 0.0);

  tau = 0; gamma = 0;
  fill_n(beta, d_n1, 0.0);

  for (size_t s = 0; s != d_S; ++s)
  {
    double prob = probs[s];
    vector<double> &ws = omega[s];
        // compute rhs, update subproblem
    double rhs[d_m2];          // rhs vector of subproblem (c-style array)
    for (size_t row = 0; row!= d_m2; ++row) // compute element-by-element
      rhs[row] = ws[row] - Tx[row];
    
    d_ald.update(rhs);   
    Ald::AldCut cut = d_ald.solve(x, ws, maxRounds);  
    tau += prob * cut.coef_eta;
    gamma += prob * cut.cons;
    
    for (size_t var = 0; var != d_n1; ++var)
      beta[var] += prob * cut.coef_x[var];   
      
  }
}