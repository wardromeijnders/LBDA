#include "problem.h"

void Problem::randomInstance(int A_low, int A_high ,
                        int T_low, int T_high,
                        int W_low, int W_high,
                        int c_low, int c_high,
                        int b_low, int b_high,
                        int q_low, int q_high)
{
  clear_sub();
  
  d_Amat = d_gen.rand_unif_mat(d_m1, d_n1, A_low, A_high);
  d_Tmat = d_gen.rand_unif_mat(d_m2, d_n1, T_low, T_high);
  d_Wmat = d_gen.rand_unif_mat(d_m2, d_n2, W_low, W_high);
  
  d_b = d_gen.rand_unif_vec(d_m1, b_low, b_high);  
  d_c = d_gen.rand_unif_vec(d_n1, c_low, c_high);
  d_q = d_gen.rand_unif_vec(d_n2, q_low, q_high);
}