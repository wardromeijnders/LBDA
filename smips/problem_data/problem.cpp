#include "problem.h"

Problem::Problem(size_t n1, size_t p1, size_t m1, 
            size_t n2, size_t p2, size_t m2, size_t S, 
            Data &generator, GRBEnv &env,
            size_t fs_leq, size_t fs_geq,
            size_t ss_leq, size_t ss_geq)
:
d_n1(n1), d_p1(p1), d_m1(m1), d_n2(n2), d_p2(p2), d_m2(m2), d_S(S),
d_probs(S, 1.0 / S),
d_fs_leq(fs_leq), d_fs_geq(fs_geq), 
d_ss_leq(ss_leq), d_ss_geq(ss_geq),
d_l1(n1, 0.0), d_u1(n1, 1e20),
d_l2(n2, 0.0), d_u2(n2, 1e20),
d_gen(generator),
d_sub(env),
d_sub_initialized(false),
d_L(0)
{}