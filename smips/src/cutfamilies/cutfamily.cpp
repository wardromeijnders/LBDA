#include "cutfamilies/cutfamily.h"


CutFamily::CutFamily(GRBEnv &env, const Problem &problem) :
    d_env(env), d_model(env), d_problem(problem)
{
}

double CutFamily::solve()
{
    d_model.optimize();
    return d_model.get(GRB_DoubleAttr_ObjVal);
}
