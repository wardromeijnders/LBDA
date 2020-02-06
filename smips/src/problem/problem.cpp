#include "problem.h"

Problem::Problem(size_t n1,
                 size_t p1,
                 size_t m1,
                 size_t n2,
                 size_t p2,
                 size_t m2,
                 size_t S,
                 Data &generator,
                 GRBEnv &env,
                 size_t fs_leq,
                 size_t fs_geq,
                 size_t ss_leq,
                 size_t ss_geq) :
    d_gen(generator),
    d_sub(env),
    d_sub_initialized(false),
    d_m1(m1),
    d_m2(m2),
    d_n1(n1),
    d_n2(n2),
    d_p1(p1),
    d_p2(p2),
    d_S(S),
    d_L(0),
    d_fs_leq(fs_leq),
    d_fs_geq(fs_geq),
    d_ss_leq(ss_leq),
    d_ss_geq(ss_geq),
    d_l1(arma::zeros(n1)),
    d_u1(n1),
    d_l2(arma::zeros(n2)),
    d_u2(n2),
    d_probs(S)
{
    d_u1.fill(arma::datum::inf);
    d_u2.fill(arma::datum::inf);

    d_probs.fill(1.0 / S);
}
