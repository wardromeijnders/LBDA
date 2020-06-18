#include "decompositions/decomposition.h"


Decomposition::Decomposition(GRBEnv &env, Problem const &problem) :
    d_env(env), d_model(env), d_problem(problem)
{
}

double Decomposition::solve()
{
    d_model.optimize();
    return d_model.get(GRB_DoubleAttr_ObjVal);
}
