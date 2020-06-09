#include "problem.h"
#include "smps/smps.h"


Problem Problem::fromSmps(char const *location, GRBEnv &env)
{
    smps::Smps smps;
    smps.readSmps(location);

    Problem problem{env};

    problem.d_Amat = smps.Amat();
    problem.d_Wmat = smps.Wmat();
    problem.d_Tmat = smps.Tmat();

    // TODO

    return problem;
}
