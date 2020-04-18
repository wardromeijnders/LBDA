#include "decompositions/lpdual.h"

LpDual::LpDual(GRBEnv &env, Problem const &problem) :
    Decomposition(env, problem)
{
}
