#include "problem.h"

void Problem::ssv95(size_t S, bool fs_continuous, bool ss_binary, bool standard_T)
{ 
  clear_sub();
  
  d_n1 = 2; d_m1 = 0; d_fs_leq = 0; d_fs_geq = 0;
  d_p1 = fs_continuous? 0 : 2;
  d_n2 = 4; d_p2 = 4; d_m2 = 2; d_ss_leq = 2; d_ss_geq = 0;
  d_S = S * S;
  
  vector<double> l1(d_n1, 0.0);  
  vector<double> l2(d_n2, 0.0);  
  vector<double> u1(d_n1, 5.0);   
  double ub = ss_binary ? 1.0 : 1e20; 
  vector<double> u2(d_n2, ub);  
  
  vector<double> fs_costs = {-1.5, -4};
  vector<double> ss_costs = {-16, -19, -23, -28};  
  vector<vector<double>> T_standard = {{1, 0}, {0, 1}};
  vector<vector<double>> T_alternative = {{2.0/3, 1.0/3}, {1.0/3, 2.0/3}};
  vector<vector<double>> Wmat = {{2, 3, 4, 5}, {6, 1, 3, 2}};
  
  vector<vector<double>> omega(d_S);
  
  double jump = 10.0 / (S - 1);
  vector<double> omega_vals(S);
  for (size_t s = 0; s != S; ++s)
    omega_vals[s] = 5 + s * jump;
  
  for (size_t s1 = 0; s1 != S; ++s1)
  {
    for (size_t s2 = 0; s2 != S; ++s2)
    {
      size_t s = s1 * S + s2;
      omega[s].push_back(omega_vals[s1]);
      omega[s].push_back(omega_vals[s2]);
    }
  }
  
  vector<double> probs(d_S, 1.0 / d_S);

  d_L = -320;
  
  d_l1 = l1; 
  d_l2 = l2;  
  d_u1 = u1;
  d_u2 = u2;
  
  d_c = fs_costs;
  d_q = ss_costs;
  d_Wmat = Wmat;
  d_Tmat = standard_T ? T_standard : T_alternative;
  d_omega = omega;
  d_probs = probs;

}