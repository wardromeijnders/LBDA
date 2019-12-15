#include "master.h"

#include <string>

Master::Solution Master::solve()
{
    // d_model.optimize();
    // d_theta and d_xVars are updated correspondingly (thank Gurobi for that)
    GRBoptimize(d_cmodel);

    Solution sol;

    sol.xVals = new double[d_n1];
    GRBgetdblattrarray(d_cmodel, "X", 1, d_n1, sol.xVals);

    GRBgetdblattrelement(d_cmodel, "X", 0, &(sol.thetaVal));
    return sol;
}
