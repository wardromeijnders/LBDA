#include "main.h"

#include <cassert>

int main(int argc, char **argv)
try
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
    auto master3 = MasterProblem(master);
    auto ptr = master3.solve(sbDecomposition);
    auto res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';
    assert(std::abs(problem.evaluate(res)) - 59.8893 <= 0.001);

    GRBfreeenv(c_env);
}
catch (...)
{
    std::cerr << "Unexpected error, terminating.\n";
}
