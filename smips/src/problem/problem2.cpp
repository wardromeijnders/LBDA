#include "problem.h"

Problem::Problem(Data &generator, GRBEnv &env) :
    d_L(0),
    d_gen(generator),
    d_sub(env),
    d_sub_initialized(false)
{
}