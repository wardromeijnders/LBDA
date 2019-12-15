#include "benders.h"

Benders::Benders(GRBEnv &env, GRBenv *c_env, Problem &problem) :
    d_problem(problem),
    d_env(env),
    d_n1(problem.d_n1),
    d_m2(problem.d_m2),
    d_n2(problem.d_n2),
    d_S(problem.d_S),
    d_master(env, c_env, problem),
    d_sub(env, problem),
    d_lr(env, problem),
    d_gomory(env, problem),
    d_ald(c_env, problem),
    d_pslp(env, c_env, problem),
    d_nCuts(0),
    d_runTime(0),
    d_visited(problem.d_S),
    d_objectives(problem.d_S)
{
    d_xvals = new double[d_n1];
}
