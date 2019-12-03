#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>

#include "gurobi_c++.h"
#include "gurobi_c.h"

#include "smips/problem_data/problem.h"
#include "smips/problem_data/data.h"
#include "smips/algorithms/deqform/deqform.h"
#include "smips/algorithms/decomposition/benders.h"

void run_ssv(Data &rand, GRBEnv &env, GRBenv *c_env, bool fs_cont, bool ss_bin, bool standard_T)
{
  cout << "continuous first stage: " << fs_cont << '\n';
  cout << "binary second stage: " << ss_bin << '\n';
  cout << "T = I: " << standard_T << "\n\n";

  Problem problem(rand, env);
  
  vector<size_t> nPoints = {2, 3, 6, 11, 21, 41, 101};
  size_t n1 = 2;
  size_t m2 = 2;
  
  size_t nAlphas = 100;
  size_t max_iter = 10;

  for (size_t point_idx = 0; point_idx != nPoints.size(); ++point_idx)
  {
    problem.ssv95(nPoints[point_idx], fs_cont, ss_bin, standard_T);    // creating problem
    vector<vector<double>> &Tmat = problem.d_Tmat;
    
          /* solving DEF */
    DeqForm DEF(env, problem);                               
    DEF.solve(7200.0);
    double x_def[n1];                                        
    copy_n(DEF.d_xVals, n1, x_def);
    double DEF_runTime = DEF.d_runTime;
    double lb = DEF.d_objBound;
    double ub = DEF.d_objVal;
          
          /* l-shaped */ 
    Benders lshaped(env, c_env, problem);                    // solve LP-relaxation
    lshaped.lpSolve();
    double x_lp[n1];
    copy_n(lshaped.d_xvals, n1, x_lp);   
    double g_xlp = problem.evaluate(x_lp);                   // evaluate   
    size_t lshaped_cuts = lshaped.d_nCuts;
    double lshaped_runTime = lshaped.d_runTime;
        
        /* lbda with alpha = 0 */
    double alpha[m2];
    fill_n(alpha, m2, 0.0);    
    Benders lbda_zero = lshaped;
    lbda_zero.lbda(alpha, 1.0);
    double x_zero[n1];
    copy_n(lbda_zero.d_xvals, n1, x_zero);
    double g_x_zero = problem.evaluate(x_zero);
    size_t lbda_zero_cuts = lbda_zero.d_nCuts;
    double lbda_zero_runTime = lbda_zero.d_runTime;
    
        /* lbda with alpha = Tx_DEF */
    fill_n(alpha, m2, 0.0);
    for (size_t row = 0; row != m2; ++row)
    {
      for (size_t var = 0; var != n1; ++var)
        alpha[row] += Tmat[row][var] * x_def[var]; 
    }   
    Benders lbda_xdef = lshaped;
    lbda_xdef.lbda(alpha, 1.0);
    double x_xdef[n1];
    copy_n(lbda_xdef.d_xvals, n1, x_xdef);
    double g_x_xdef = problem.evaluate(x_xdef);
    size_t lbda_xdef_cuts = lbda_xdef.d_nCuts;
    double lbda_xdef_runTime = lbda_xdef.d_runTime;
    
       /*lbda with alpha = Txlp */
    for (size_t row = 0; row != m2; ++row)
    {
      for (size_t var = 0; var != n1; ++var)
        alpha[row] += Tmat[row][var] * x_lp[var]; 
    }     
    Benders lbda_xlp = lshaped;
    lbda_xlp.lbda(alpha, 1.0);
    double x_xlp[n1];
    copy_n(lbda_xlp.d_xvals, n1, x_xlp);
    double g_x_xlp = problem.evaluate(x_xlp);
    size_t lbda_xlp_cuts = lbda_xlp.d_nCuts;
    double lbda_xlp_runTime = lbda_xlp.d_runTime;
    
    
        /* lbda with 100 different alphas */
    double g_x_plus = g_x_zero;
    double lbda_xplus_runTime = lshaped.d_runTime;
    for (size_t alpha_idx = 0; alpha_idx != nAlphas; ++alpha_idx)
    {
      vector<double> alpha_vector = rand.unif_real_vec(m2, 0, 5);
      Benders lbda = lshaped;
      lbda.lbda(alpha_vector.data(), 1.0);
      double x_alpha[n1];
      copy_n(lbda.d_xvals, n1, x_alpha);
      double g_x_alpha = problem.evaluate(x_alpha);
      g_x_plus = min(g_x_plus, g_x_alpha);   
      lbda_xplus_runTime += (lbda.d_runTime - lshaped.d_runTime);           
    }
        /* iterative scheme */
    double x_current[n1];              // alpha = Tx_current
    copy_n(x_zero, n1, x_current);      // initalized by Tx_zero 
    double g_x_iterative = g_x_zero;
    double lbda_x_iterative_runTime = lbda_zero_runTime;
    for (size_t iter = 0; iter != max_iter; ++iter)
    {
      fill_n(alpha, m2, 0.0);
      for (size_t row = 0; row != m2; ++row)
      {
        for (size_t var = 0; var != n1; ++var)
          alpha[row] += Tmat[row][var] * x_current[var]; 
      }   
      Benders lbda = lshaped;
      lbda.lbda(alpha, 1.0);
      copy_n(lbda.d_xvals, n1, x_current);
      double g_x_current = problem.evaluate(x_current);
      g_x_iterative = min(g_x_iterative, g_x_current);
      lbda_x_iterative_runTime += (lbda.d_runTime - lshaped.d_runTime);
    }
  

    
    cout << lb << ' ' << ub << ' ' << g_xlp << ' ' << g_x_zero << ' ' << g_x_xdef << ' ' << g_x_xlp << ' ' << g_x_plus << ' ' << g_x_iterative << '\n';
    cout << DEF_runTime << ' ' << lshaped_runTime << ' ' << lbda_zero_runTime << ' ' << lbda_xdef_runTime << ' ' << lbda_xlp_runTime << ' ' << lbda_xplus_runTime << ' ' << lbda_x_iterative_runTime << "\n\n";   
         
  } 

  

  
}











