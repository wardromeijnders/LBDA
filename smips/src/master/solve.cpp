#include "master.h"


Master::Solution const Master::solve()
{
    GRBoptimize(d_cmodel);

    auto xVals = new double[d_n1];
    GRBgetdblattrarray(d_cmodel, "X", 1, d_n1, xVals);

    double theta;
    GRBgetdblattrelement(d_cmodel, "X", 0, &theta);

    return Solution{xVals, theta};
}
