#include "subproblem.h"

SubProblem::SubProblem(SubProblem const &other) :
    d_model(other.d_model),
    d_problem(other.d_problem)
{
    d_vars = d_model.getVars();
    d_constrs = d_model.getConstrs();
}
