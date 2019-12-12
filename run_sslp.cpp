#include "gurobi_c++.h"
#include "gurobi_c.h"
#include "smips/algorithms/decomposition/benders.h"
#include "smips/algorithms/deqform/deqform.h"
#include "smips/problem_data/data.h"
#include "smips/problem_data/problem.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void run_sslp(Data &rand, GRBEnv &env, GRBenv *c_env)
{
    Problem problem(rand, env);

    vector<vector<size_t>> inputs = {{5, 25, 50},
                                     {5, 25, 100},
                                     {15, 45, 5},
                                     {15, 45, 10},
                                     {15, 45, 15},
                                     {10, 50, 50},
                                     {10, 50, 100},
                                     {10, 50, 500},
                                     {10, 50, 1000},
                                     {10, 50, 2000}};

    for (vector<size_t> input : inputs)
    {
        size_t nServers = input[0];
        size_t nClients = input[1];
        size_t nScenarios = input[2];
        cout << "sslp_" << nServers << '_' << nClients << '_' << nScenarios
             << '\n';
        problem.sslp(nServers, nClients, nScenarios);  // creating problem

        size_t n1 = problem.d_n1;
        size_t m2 = problem.d_m2;

        /* l-shaped */
        Benders lshaped(env, c_env, problem);  // solve LP-relaxation
        lshaped.lpSolve();
        double x_lp[n1];
        copy_n(lshaped.d_xvals, n1, x_lp);
        double g_xlp = problem.evaluate(x_lp);  // evaluate
        size_t lshaped_cuts = lshaped.d_nCuts;
        double lshaped_runTime = lshaped.d_runTime;


        /* lbda with alpha = 0 */
        double alpha[m2];
        fill_n(alpha, m2, 0.0);

        Benders lbda_zero = lshaped;
        lbda_zero.lbda(alpha, 1.0);
        double x_zero[n1];
        copy_n(lbda_zero.d_xvals, n1, x_zero);
        double g_x_zero = problem.evaluate(x_zero);
        size_t lbda_zero_cuts = lbda_zero.d_nCuts;
        double lbda_zero_runTime = lbda_zero.d_runTime;


        /* lbda with alpha = Txlp */
        vector<vector<double>> &Tmat = problem.d_Tmat;
        for (size_t row = 0; row != m2; ++row)
        {
            for (size_t var = 0; var != n1; ++var)
                alpha[row] += Tmat[row][var] * x_lp[var];
        }

        Benders lbda_xlp = lshaped;
        lbda_xlp.lbda(alpha, 1.0);
        double x_xlp[n1];
        copy_n(lbda_xlp.d_xvals, n1, x_xlp);
        double g_x_xlp = problem.evaluate(x_xlp);
        size_t lbda_xlp_cuts = lbda_xlp.d_nCuts;
        double lbda_xlp_runTime = lbda_xlp.d_runTime;


        cout << g_xlp << ' ' << g_x_zero << ' ' << g_x_xlp << ' '
             << lshaped_runTime << ' ' << lbda_zero_runTime << ' '
             << lbda_xlp_runTime << ' ' << lshaped_cuts << ' ' << lbda_zero_cuts
             << ' ' << lbda_xlp_cuts << '\n';
    }
}
