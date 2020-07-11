#include "main.h"

int main(int argc, char **argv)
try
{
    // TODO clean this up into several methods, document CLI better.

    Sarge sarge;

    // clang-format off
    sarge.setArgument("h", "help", "Prints this help text.", false);
    sarge.setArgument("c", "cut", "Cut family to use if method=cut. One of {lp, sb, lbda}.", true);
    sarge.setArgument("t", "time", "Time limit if method=deq.", false);

    sarge.setDescription("SMIPS. A program for solving two-stage mixed-integer stochastic programs.");
    sarge.setUsage("smips [-h] [-c | -t] <file> <method>");
    // clang-format on

    if (!sarge.parseArguments(argc, argv))
    {
        std::cerr << "Could not parse arguments.\n";
        return EXIT_FAILURE;
    }

    if (sarge.exists("help"))
    {
        sarge.printHelp();
        return EXIT_SUCCESS;
    }

    std::string file;

    if (!sarge.getTextArgument(0, file))
    {
        std::cerr << "Did not receive an SMPS file location.\n";
        return EXIT_FAILURE;
    }

    std::string method;

    if (!sarge.getTextArgument(1, method))
    {
        std::cerr << "Did not receive a solution method.\n";
        return EXIT_FAILURE;
    }

    GRBEnv env;
    auto problem = Problem::fromSmps(file);

    if (method == "deq")
    {
        std::cout << "Deterministic equivalent:\n";
        DeterministicEquivalent deq{env, problem};

        auto ptr = deq.solve(120);  // TODO make time limit CLI argument
        auto res = *ptr;

        std::cout << "x = \n" << res << '\n';
        std::cout << "Objective = " << deq.objective() << '\n';
        std::cout << "    c^T x = " << deq.firstStageObjective() << '\n';
        std::cout << "     Q(x) = " << deq.secondStageObjective() << '\n';
    }

    // TODO use selection mechanism, what about the alpha's for LBDA?
    if (method == "cut")
    {
        std::cout << "Decomposition with cut family:\n";
        MasterProblem master{env, problem};
        StrongBenders cutFamily{env, problem};

        auto ptr = master.solveWith(cutFamily);
        auto res = *ptr;

        std::cout << "x = \n" << res << '\n';
        std::cout << "Objective = " << master.objective() << '\n';
        std::cout << "    c^T x = " << master.firstStageObjective() << '\n';
        std::cout << "     Q(x) = " << master.secondStageObjective() << '\n';
    }
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
