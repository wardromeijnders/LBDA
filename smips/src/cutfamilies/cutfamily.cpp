#include "cutfamilies/cutfamily.h"


CutFamily::CutFamily(ProblemData const &problem) :
    d_model(d_env), d_problem(problem), d_sub(problem)
{
}

double CutFamily::solve()
{
    d_model.optimize();
    return d_model.get(GRB_DoubleAttr_ObjVal);
}
