#include "masterproblem.h"

MasterProblem::~MasterProblem()
{
    GRBfreemodel(d_cmodel);
}
