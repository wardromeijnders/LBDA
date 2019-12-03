#include "ald.h"

Ald::AldCut Ald::solve(double *xbar, vector<double> omega, size_t maxRounds)
{
  size_t nVars = d_n2;
  size_t nConstrs = d_m2;
  double yvals[d_p2];

  vector<vector<double>> Tmat = d_Tmat; // cuts have to be appended
  vector<double> tau(nConstrs, 0);

  bool stop = false;
   
  size_t round = 0; 
  while (not stop)
  {
    ++round;
    stop = true;
    GRBoptimize(d_model);                   // solve the model
    

    if (round >= maxRounds)
      break;
    
    int bhead[nConstrs];
    GRBgetBasisHead(d_model, bhead);        // extract basis info   
    GRBgetdblattrarray(d_model, GRB_DBL_ATTR_X, 0, d_p2, yvals);    // extract values of integer variables

    
        
    size_t nCuts = 0;
        // adding a round of cuts
    GRBsvec e_i;      e_i.len = 1;                                                                            double e_i_val[1] = {1.0}; e_i.val = e_i_val;
    GRBsvec B_row;    B_row.len = nConstrs;            int B_row_ind[nConstrs]; B_row.ind = B_row_ind;        double B_row_val[nConstrs]; B_row.val = B_row_val;
    GRBsvec tableau;  tableau.len = nVars + nConstrs;  int tab_ind[nVars + nConstrs]; tableau.ind = tab_ind;  double tab_val[nVars + nConstrs]; tableau.val = tab_val;    


    for (size_t row = 0; row != nConstrs; ++row)    // loop over rows of simplex tableau 
    {
      int basic_var = bhead[row];            // index of corresponding basic variable
      if (basic_var >= d_p2)                 // check if variable has to be integer
        continue;                            // if not, do not derive a cut
      
      double yval = yvals[basic_var]; 
      if (is_integer(yval))                  // if variable value is integer,
        continue;                            // then do not derive a cut
        
      stop = false;                          // solution is not integer, derive cut     

      double coef_x[d_n1]; double coef_y[nVars]; double coef_eta; double coef_rhs;  // cut coefficients
      gmi_cut(row, yval, xbar, nConstrs, nVars, e_i, B_row, tableau, Tmat, tau, coef_x, coef_y, coef_rhs, coef_eta); // compute cut coefficients 


      add_cut(nVars, nCuts, coef_x, coef_y, coef_eta, coef_rhs, Tmat, tau, omega);
      ++nCuts;
    }   
      
    nConstrs += nCuts;
    nVars += nCuts;      // because we added slacks

  } 
  
  double lambda[nConstrs];
  GRBgetdblattrarray(d_model, "Pi", 0, nConstrs, lambda);
  
  double cons = 0.0; double coef_eta = 0.0; vector<double> coef_x(d_n1); 
  fill_n(coef_x.begin(), d_n1, 0.0);
  for (size_t con = 0; con != nConstrs; ++con)
  {
    double pi = lambda[con];
    coef_eta -= pi * tau[con];
    cons += pi * omega[con];
    for (size_t var = 0; var != d_n1; ++var)
      coef_x[var] -= pi * Tmat[con][var];
  }
  clean();    // remove cuts and corresponding slacks  
  
  return AldCut {cons, coef_eta, coef_x};
}

