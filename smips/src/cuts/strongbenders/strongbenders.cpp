#include "cuts/strongbenders.h"


StrongBenders::StrongBenders(GRBEnv &env, Problem const &problem) :
    Cut(env, problem),
    d_lr(env, problem)
{
}
