#include "benders.h"

#include <chrono>

void Benders::lpSolve(double tol)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    bool stop = false;
    size_t iter = 0;

    while (not stop)
    {
        // cout << "iteration: " << iter << '\n';
        ++iter;

        // solve master problem, and collect x and theta
        auto sol = d_master.solve();

        double *x = sol.xVals;
        double theta = sol.thetaVal;

        // derive cut
        double beta[d_n1];
        double gamma;
        lpCut(x, beta, gamma);

        // add the cut (conditional on it being violated by the current
        // solution)
        stop = d_master.addCut(beta, gamma, x, theta, tol);

        if (stop)
            std::copy(x, x + d_n1, d_xvals);

        delete[] x;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    d_runTime += std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)
                     .count()
                 / 1000.0;
    d_nCuts += iter - 1;

    // cout << "Number of L-shaped iterations: " << iter << '\n';
}
