#include "benders.h"


std::unique_ptr<arma::vec> Benders::solve(Cut &cut, double tol)
{
    using seconds = std::chrono::seconds;
    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    size_t iter = 1;

    while (true)
    {
        ++iter;

        auto sol = d_master.solve();
        auto cutResult = cut.computeCut(*sol.x);

        if (MasterProblem::isValidCut(cutResult, sol, tol))
            d_master.addCut(cutResult);
        else
        {
            auto t2 = clock::now();

            d_runTime += std::chrono::duration_cast<seconds>(t2 - t1).count();
            d_nCuts += iter;

            return std::move(sol.x);
        }
    }
}
