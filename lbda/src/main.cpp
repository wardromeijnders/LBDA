#include "main.h"

int main(int argc, char **argv)
try
{
    auto arguments = parseArguments(argc, argv);

    if (arguments.printUsage)
    {
        std::cout << USAGE << '\n';
        return EXIT_SUCCESS;
    }

    auto problem = ProblemData::fromSmps(arguments.file);

    switch (arguments.methodType)
    {
        case Arguments::DECOMPOSITION:
        {
            MasterProblem master(problem, arguments.lb, arguments.ub);
            CutFamily *cutFamily;

            switch (arguments.cutType)
            {
                case Arguments::LP_DUAL:
                    cutFamily = new LpDual(problem);
                    break;
                case Arguments::STRONG_BENDERS:
                    cutFamily = new StrongBenders(problem);
                    break;
                case Arguments::LOOSE_BENDERS:
                default:
                    // TODO how to set alpha from the command line?
                    arma::vec alpha = arma::zeros(problem.Wmat().n_cols);
                    cutFamily = new LooseBenders(problem,
                                                 alpha,
                                                 arguments.timeLimit);
                    break;
            }

            auto solution = master.solveWith(*cutFamily);
            auto decisions = *solution;

            delete cutFamily;

            printSolution(decisions, master);
            break;
        }
        case Arguments::DETERMINISTIC_EQUIVALENT:
        {
            DeterministicEquivalent deq(problem);

            auto solution = deq.solve(arguments.timeLimit);
            auto decisions = *solution;

            printSolution(decisions, deq);

            std::cout << "Gap (%) = " << deq.mipGap() << "%\n";
            break;
        }
    }
}
catch (GRBException const &e)
{
    std::cerr << e.getMessage() << '\n';
    return EXIT_FAILURE;
}
catch (std::exception const &e)
{
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "Something went wrong.\n";
    return EXIT_FAILURE;
}

argument_t parseArguments(int argc, char **argv)
{
    argument_t arguments;
    int option;

    while ((option = getopt(argc, argv, "hm:c:l:u:t:")) != -1)
    {
        switch (option)
        {
            case 'h':
                // If this flag is set, it is all we will process - immediately
                // after, the program ends. No need to continue processing.
                arguments.printUsage = true;
                return arguments;
            case 'm':
                if (std::strcmp(optarg, "deq") == 0)
                    arguments.methodType = arguments.DETERMINISTIC_EQUIVALENT;

                break;
            case 'c':
                if (std::strcmp(optarg, "lp") == 0)
                    arguments.cutType = arguments.LP_DUAL;

                if (std::strcmp(optarg, "sb") == 0)
                    arguments.cutType = arguments.STRONG_BENDERS;

                break;
            case 'l':
                arguments.lb = std::stod(optarg);
                break;
            case 'u':
                arguments.ub = std::stod(optarg);
                break;
            case 't':
                arguments.timeLimit = std::stod(optarg);
                break;
            default:
                break;  // unexpected option, but not the end of the world.
        }
    }

    if (argc == optind)
        throw std::runtime_error("Did not receive an SMPS file location.");

    arguments.file = argv[optind];
    return arguments;
}


template<class T> void printSolution(arma::vec const &decisions, T &method)
{
    std::cout << "x = \n" << decisions << '\n';
    std::cout << "Obj.    = " << method.objective() << '\n';
    std::cout << "c^T x   = " << method.firstStageObjective() << '\n';
    std::cout << "Q(x)    = " << method.secondStageObjective() << '\n';
}
