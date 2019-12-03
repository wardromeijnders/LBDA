#include "deqform.h"

DeqForm::DeqForm(GRBEnv &env, Problem &problem)
:
d_status(1),
d_model(GRBModel(env))
{
  size_t n1 = problem.d_n1;   
  d_n1 = n1;
  d_MIP = (problem.d_p1 == 0 && problem.d_p2 == 0) ? false: true;  
  
  init_fs(n1, problem.d_p1, problem.d_m1, 
          problem.d_fs_leq, problem.d_fs_geq,
          problem.d_l1.data(), problem.d_u1.data(),
          problem.d_c.data(), problem.d_b.data(),
          problem.d_Amat);

  init_ss(n1, problem.d_n2, problem.d_p2, problem.d_m2, 
          problem.d_S, problem.d_ss_leq, problem.d_ss_geq, 
          problem.d_l2.data(), problem.d_u2.data(),
          problem.d_probs.data(), problem.d_q.data(),
          problem.d_Tmat, problem.d_Wmat, problem.d_omega);          
}






















