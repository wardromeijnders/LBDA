#include "main.h"

int main(int argc, char **argv)
{
    // TODO check all output numbers (verification).
    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    env.set(GRB_IntParam_Threads, 1);

    Problem problem = Problem::fromSmps(argv[1], env);
    MasterProblem master{env, problem};

    // TODO CLI selection of decomposition strategy?
    StrongBenders decomposition{env, problem};
    auto ptr = master.solve(decomposition);
    auto res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';
}
