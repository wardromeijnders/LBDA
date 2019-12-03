#include "ald.h"

void Ald::gmi_cut(int row, double yval, double *xbar, size_t nConstrs, size_t nVars,
                          GRBsvec &e_i, GRBsvec &B_row, GRBsvec &tableau, 
                          vector<vector<double>> &Tmat, vector<double>& tau, 
                          double *coef_x, double *coef_y, double &coef_rhs, double &coef_eta)
{
  fill_n(coef_x, d_n1, 0.0); fill_n(coef_y, nVars, 0.0);    // initialize
  coef_rhs = 1; coef_eta = 0;
   
      
  int e_i_ind[1] = {row};              // unit vector   
  e_i.ind = e_i_ind; 
  
  GRBBSolve(d_model, &e_i, &B_row);          // extract i-th row of B^-1
  GRBBinvRowi(d_model, row, &tableau); // extract i-th simplex tableau row


  double BinvT_row[d_n1];                // compute i-th row of B^-1 T

  for (size_t var = 0; var != d_n1; ++var)
  {
    BinvT_row[var] = 0.0;
    for (size_t con = 0; con != B_row.len; ++con)
      BinvT_row[var] += B_row.val[con] * Tmat[B_row.ind[con]][var];  
  }
  
  

  double BinvTe = -accumulate(BinvT_row, BinvT_row + d_n1, 0.0);  // compute -B^-1Te + B^-1 tau
  
 
  for (size_t con = 0; con != B_row.len; ++con)
    BinvTe += B_row.val[con] * tau[B_row.ind[con]];     
  
   
          
          // computing cut coefficients     
  double f0 = yval - floor(yval);   
     
  for (size_t var = 0; var != d_n1; ++var)
  {
    double val = BinvT_row[var];
    double coef = max(val/f0, -val/(1-f0) );
    coef_x[var] += coef;
    coef_rhs += coef * xbar[var];
    coef_eta += coef;
  }
  
  

  
  
  coef_eta += max(BinvTe/f0, -BinvTe/(1-f0) );
  
  int bhead[nConstrs];
  GRBgetBasisHead(d_model, bhead);        // extract basis info  
  for (size_t idx = 0; idx != tableau.len; ++idx)
  {
    int var = tableau.ind[idx];      // variable index of non-zero in tableau row  
    double val = tableau.val[idx];

    
    // check if variable is basic (if so it does not contribute to the coefficients)
    bool is_basic = false;

    for (size_t con = 0; con != nConstrs; ++con)
    {
      if (bhead[con] == var)
      {
        is_basic = true;
        continue;
      }    
    } 
    if (is_basic)
      continue;
    double fi = val - floor(val);

    if (var < d_p2)      // check if integer variable
      coef_y[var] += min(fi/f0, (1-fi)/(1-f0) );
    

      
    if (var >= d_p2 && var < nVars)
      coef_y[var] += max(val/f0, -val/(1-f0) );      

  } 

     
}   






