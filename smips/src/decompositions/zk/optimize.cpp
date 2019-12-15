#include "decompositions/zk.h"

void ZK::optimize()
{
    GRBoptimize(d_model);
}
