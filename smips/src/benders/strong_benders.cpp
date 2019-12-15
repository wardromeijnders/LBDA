#include "benders.h"

void Benders::strong_benders(double tol)
{
    bool stop = false;
    size_t iterations = 0;

    while (not stop)
    {
        ++iterations;

        // solve master problem, and collect x and theta
        Master::Solution sol = d_master.solve();

        double *x = sol.xVals;
        double theta = sol.thetaVal;

        // derive cut
        double beta[d_n1];
        double gamma;
        sb_cut(x, beta, gamma);  // beta and gamma are RBA

        // add the cut conditional on it being violated by the current solution
        stop = d_master.addCut(
            beta,
            gamma,
            x,
            theta,
            tol);  // if no cut was added, then while loop is exited

        if (stop)
            std::copy(x, x + d_n1, d_xvals);

        delete[] x;
    }

    std::cout << "Number of strong benders cuts: " << iterations << '\n';
}
