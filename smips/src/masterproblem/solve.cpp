#include "masterproblem.h"


std::unique_ptr<arma::vec> MasterProblem::solve(Decomposition &decomposition,
                                                double tol)
{
    while (true)
    {
        GRBoptimize(d_cmodel);

        size_t const n1 = d_problem.Amat().n_rows;

        auto xVals = std::make_unique<arma::vec>(n1);
        GRBgetdblattrarray(d_cmodel, "X", 1, n1, xVals->memptr());

        double theta;
        GRBgetdblattrelement(d_cmodel, "X", 0, &theta);

        auto cut = decomposition.computeCut(*xVals);

        // Is the proposed cut violated by the current solution?
        if (cut.gamma + arma::dot(*xVals, cut.beta) >= theta + tol)
            addCut(cut);
        else
            return xVals;
    }
}
