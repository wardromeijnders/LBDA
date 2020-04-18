#include "problem.h"

Problem::~Problem()
{
    if (d_isSubProblemInitialised)
        delete[] d_constrs;
}
