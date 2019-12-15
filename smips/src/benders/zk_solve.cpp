#include "benders.h"

void Benders::zk_solve(bool strong, double tau_bar, double tol, size_t maxRounds)
{
    bool stop = false;
    size_t iter = 0;

    d_pslp.update_tau(tau_bar);

    while (not stop)
    {
        std::cout << "\nzk iteration: " << iter << '\n';
        ++iter;

        // solve master problem, and collect x and theta
        Master::Solution sol = d_master.solve();

        double *x = sol.xVals;

        std::cout << "current solution: ";
        for (size_t var = 0; var != d_n1; ++var)
            std::cout << x[var] << ' ';
        std::cout << '\n';

        double cx = 0;
        for (size_t var = 0; var != d_n1; ++var)
            cx += d_problem.d_c[var] * x[var];
        double Q = d_problem.evaluate(x) - cx;


        double theta = sol.thetaVal;

        // derive zk cut
        double beta[d_n1];
        double gamma;
        double kappa;  // kappa * theta >= beta^T x + gamma

        if (strong)
            d_pslp.strong_zk_cut(x,
                                 theta,
                                 d_master,
                                 beta,
                                 gamma,
                                 kappa,
                                 maxRounds);
        else
            d_pslp.zk_cut(x, theta, d_master, beta, gamma, kappa, maxRounds);

        stop = d_master.add_zk_cut(beta, gamma, kappa, x, theta, tol);

        std::cout << "Q(x) = " << Q << '\n';

        if (stop)
            std::copy(x, x + d_n1, d_xvals);

        delete[] x;
    }

    std::cout << "Number of zk cuts: " << iter << '\n';
}