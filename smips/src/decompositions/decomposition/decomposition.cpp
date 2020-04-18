#include "decompositions/decomposition.h"


Decomposition::Decomposition(GRBEnv &env, Problem const &problem) :
    d_env(env),
    d_model(env),
    d_problem(problem)
{
}
