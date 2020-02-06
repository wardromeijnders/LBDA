#include "masterproblem.h"


MasterProblem::Solution const MasterProblem::solve()
{
    GRBoptimize(d_cmodel);

    auto xVals = std::make_unique<arma::vec>(d_n1);
    GRBgetdblattrarray(d_cmodel, "X", 1, d_n1, xVals->memptr());

    double theta;
    GRBgetdblattrelement(d_cmodel, "X", 0, &theta);

    return Solution{std::move(xVals), theta};
}
