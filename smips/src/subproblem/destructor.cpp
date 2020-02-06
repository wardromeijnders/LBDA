#include "subproblem.h"

SubProblem::~SubProblem()
{
    delete[] d_vars;
    delete[] d_constrs;
}
