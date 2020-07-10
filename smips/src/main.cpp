#include "main.h"

int main(int argc, char **argv)
try
{
    // TODO Make most of these parameters/choices/etc. CLI settings.
    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    env.set(GRB_IntParam_Threads, 1);

    auto problem = Problem::fromSmps(argv[1]);
    MasterProblem master{env, problem};

    DeterministicEquivalent deq{env, problem};
    auto ptr = deq.solve(120);
    auto res = *ptr;

    std::cout << "x = \n" << res << '\n';
    std::cout << "cx + Q(x) = " << deq.objective() << '\n';

    LpDual cutFamily{env, problem};
    ptr = master.solveWith(cutFamily);
    res = *ptr;

    std::cout << "x = \n" << res << '\n';
    std::cout << "cx + Q(x) = " << master.objective() << '\n';
}
catch (GRBException const &e)
{
    std::cerr << e.getMessage() << '\n';
}
catch (std::exception const &e)
{
    std::cerr << e.what() << '\n';
}
catch (...)
{
    std::cerr << "Something went wrong.\n";
}
