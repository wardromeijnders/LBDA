#include <iostream>
#include <chrono>
#include <vector>

#include "gurobi_c++.h"
#include "gurobi_c.h"

#include "smips/problem_data/problem.h"
#include "smips/problem_data/data.h"
#include "smips/algorithms/deqform/deqform.h"
#include "smips/algorithms/decomposition/benders.h"

#include <string>

#include "run.h"

using namespace std;

int main(int argc, char *argv[])
{    
  Data rand(31415);

  GRBEnv env;  
  env.set(GRB_IntParam_OutputFlag, 0); 
  env.set(GRB_IntParam_Threads, 1);
  
  GRBenv *c_env;
  GRBloadenv(&c_env, NULL);
  GRBsetintparam(c_env, "OutputFlag", 0);
  GRBsetintparam(c_env, "Threads", 1);
  
  {  

    Problem problem(rand, env);
    problem.ssv95(11, true, true, true);
    size_t n1 = problem.d_n1;
  
    double *x;
    DeqForm DEF(env, problem);   
    DEF.solve(900.0);
    x = DEF.d_xVals;
    for (size_t var = 0; var != n1; ++var)
      cout << x[var] << ' ';
    cout << '\n';
    cout << "cx + Q(x) = " << problem.evaluate(x) << '\n';
    
    Benders lshaped(env, c_env, problem);
    lshaped.lpSolve();
    x = lshaped.d_xvals; 
    for (size_t var = 0; var != n1; ++var)
      cout << x[var] << ' ';
    cout << '\n';
    double cxlpQxlp = problem.evaluate(x);
    cout << "cx + Q(x) = " << cxlpQxlp << '\n';  
    
    size_t m2 = problem.d_m2;
    double alpha[m2];
    fill_n(alpha, m2, 0); 
    
    Benders lbda = lshaped;
    lbda.lbda(alpha, 1.0);
    x = lbda.d_xvals; 
    for (size_t var = 0; var != n1; ++var)
      cout << x[var] << ' ';
    cout << '\n';
    double g_x_zero = problem.evaluate(x);
    cout << "cx + Q(x) = " << g_x_zero << '\n'; 
    
  } 

  GRBfreeenv(c_env);
}












