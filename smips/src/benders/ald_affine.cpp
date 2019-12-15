#include "benders.h"


void Benders::ald_affine(double tol, size_t maxRounds)
{
  Cglp cglp(d_env, d_master);
  
  bool stop = false;
  size_t count = 0;
  while (not stop)
  {
    ++count;

    Master::Solution sol = d_master.solve();
    double *x = sol.xVals;    
    std::cout << "x: " << *x << '\n';
    double theta = sol.thetaVal;        
      // derive ald cut
    double beta[d_n1], tau, gamma;      // ALD cut coefficients
    ald_cut(x, beta, tau, gamma, maxRounds);
    
    std::cout << "START\n";
    std::cout << "beta: " << *beta << '\n';
    std::cout << "gamma: " << gamma << '\n';
    std::cout << "tau: " << tau << '\n';

    cglp.add_disjunction(x, theta, beta, tau, gamma);
    
        // derive affine cut using disjunctive programming    
    double pi_theta, pi_x[d_n1], pi_0;   // affine cut coefficients
    cglp.solve(pi_theta, pi_x, pi_0);    // rba
    
        // transform cut coeffients to form theta >= cons + x_coefs^T x
    double x_coefs[d_n1];                  
    for (size_t var = 0; var != d_n1; ++var)
      x_coefs[var] = -pi_x[var] / pi_theta;
    double cons = pi_0 / pi_theta;     
    
    std::cout << "beta: " << x_coefs[0] << '\n';
    std::cout << "cons: " << cons << '\n';
    
    d_master.addCut(x_coefs, cons, x, theta, tol);
      
    cglp.chg_row(x_coefs, cons);    // update cglp (remove disjunctive row, add newly found row)
    
    stop = (count == 10);
    
    if (stop)
      std::copy(x, x + d_n1, d_xvals);

    delete[] x;
  }
}
