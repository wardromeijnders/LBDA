#include "main.h"

int main(int argc, char **argv)
{
    Data rand(31415);

    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    env.set(GRB_IntParam_Threads, 1);

    GRBenv *c_env;
    GRBloadenv(&c_env, NULL);
    GRBsetintparam(c_env, "OutputFlag", 0);
    GRBsetintparam(c_env, "Threads", 1);

    Problem problem(rand, env);
    problem.ssv95(11, true, true, true);
    size_t n1 = problem.d_n1;

    double *x;
    DeqForm DEF(env, problem);
    DEF.solve(900.0);
    x = DEF.d_xVals;
    for (size_t var = 0; var != n1; ++var)
        std::cout << x[var] << ' ';
    std::cout << "\ncx + Q(x) = " << problem.evaluate(x) << '\n';

    Benders lshaped(env, c_env, problem);
    lshaped.lpSolve();
    x = lshaped.d_xvals;
    for (size_t var = 0; var != n1; ++var)
        std::cout << x[var] << ' ';
    std::cout << "\ncx + Q(x) = " << problem.evaluate(x) << '\n';

    size_t m2 = problem.d_m2;
    double alpha[m2];
    std::fill_n(alpha, m2, 0);

    Benders lbda = lshaped;
    lbda.lbda(alpha, 1.0);
    x = lbda.d_xvals;
    for (size_t var = 0; var != n1; ++var)
        std::cout << x[var] << ' ';
    std::cout << "\ncx + Q(x) = " << problem.evaluate(x) << '\n';

    GRBfreeenv(c_env);
}
