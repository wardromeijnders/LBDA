#include "zk.h"

void ZK::solve(double *x, double theta, GRBmodel *master, vector<double> &kappa, vector<vector<double>> &beta, vector<double> &gamma, size_t maxRounds)
{
  double yvals[d_p2];      // to store values of integer decision vars

  bool stop = false;
  size_t round = 0;
  
  while (not stop)
  {
    GRBoptimize(d_model);                   // solve the model
    
    ++round;
    stop = true;        // resets to false if solution is non-integer
    if (round >= maxRounds)
      break;
 
    int bhead[d_nConstrs];
    GRBgetBasisHead(d_model, bhead);        // extract basis info   
    GRBgetdblattrarray(d_model, GRB_DBL_ATTR_X, 0, d_p2, yvals);    // extract values of integer variables
    
    
    size_t nCuts = 0;    
    for (size_t row = 0; row != d_nConstrs; ++row)    // loop over rows of simplex tableau 
    {
      int basic_var = bhead[row];            // index of corresponding basic variable
      if (basic_var >= d_p2)                 // check if variable has to be integer
        continue;                            // if not, do not derive a cut
      
      double yval = yvals[basic_var]; 
      if (is_integer(yval))                  // if variable value is integer,
        continue;                            // then do not derive a cut

      stop = false;                          // solution is not integer, derive cut     

      int nVarsMaster;                                      // number of variables in master problem (including slacks)
      GRBgetintattr(master, "NumVars", &nVarsMaster);

          // computing tableau row
       
          
      double tab_row_x[nVarsMaster];
      compute_tab_row_x(tab_row_x, nVarsMaster, row, master); // tableau row for (theta, x) (in that order)

      double tab_row_y[d_nVars];
      compute_tab_row_y(tab_row_y, row); // tableau row for y variables 

      double coef_x[nVarsMaster - 1]; double coef_y[d_nVars]; double coef_theta = -1; double coef_rhs = 1;// cut coefficients
      
      double a0 = yval;
      /*
      double a0 = yval + tab_row_x[0] * theta;
      for (size_t var = 1; var != nVarsMaster; ++var)
        a0 += tab_row_x[var] * x[var - 1];     
      cout << "a0 = " << a0 << " yval = " << yval << '\n';
      */

      gmi_cut(tab_row_x, tab_row_y, a0, coef_x, coef_y, coef_theta, nVarsMaster);
      transform_cut(coef_x, coef_theta, coef_rhs, kappa, beta, gamma, nVarsMaster - d_n1 - 1);

      add_cut(coef_x, coef_y, coef_theta, coef_rhs, x, theta, nVarsMaster - 1, d_nConstrs + nCuts);


     
      ++nCuts;
    }   
    d_nConstrs += nCuts;
    d_nVars += nCuts;     // slacks

  }

}