#include "ald.h"

void Ald::add_cut(size_t nVars, size_t nCuts,
                  double *coef_x, double *coef_y, double coef_eta, double coef_rhs,
                  vector<vector<double>> &Tmat, vector<double> &tau, vector<double> &omega)
{
  GRBaddvar(d_model, 0, NULL, NULL, 0.0, 0.0, 1e20, GRB_CONTINUOUS, NULL);  // slack variable
  int cind[nVars + 1];
  iota(cind, cind + nVars, 0);      // fils cind with 0,1,...,nVars-1
  cind[nVars] = nVars + nCuts;      // final element of nVars corresponds to slack variable  
  
  double cval[nVars + 1];
  copy(coef_y, coef_y + nVars, cval);
  cval[nVars] = -1;
  
  GRBaddconstr(d_model, nVars + 1, cind, cval, GRB_EQUAL, 1, NULL);
                                    // appending cut to T and tau and omega, for future cuts
  vector<double> cut_row(coef_x, coef_x + d_n1);


  Tmat.push_back(cut_row);

  
  tau.push_back(coef_eta);      
  omega.push_back(coef_rhs);            
}     