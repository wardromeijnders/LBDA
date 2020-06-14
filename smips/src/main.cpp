#include "main.h"

#include <cassert>

int main(int argc, char **argv)
{
    // TODO check all output numbers (verification).
    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    env.set(GRB_IntParam_Threads, 1);

    Problem problem = Problem::fromSmps(argv[1], env);

    GRBenv *c_env;
    GRBloadenv(&c_env, nullptr);
    GRBsetintparam(c_env, "OutputFlag", 0);
    GRBsetintparam(c_env, "Threads", 1);

    MasterProblem master{c_env, problem};

    // TODO CLI selection of decomposition strategy?
    StrongBenders sbDecomposition{env, problem};
    auto ptr = master.solve(sbDecomposition);
    auto res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';

    GRBfreeenv(c_env);
}
