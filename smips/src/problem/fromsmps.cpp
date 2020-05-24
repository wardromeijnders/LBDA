#include "problem.h"
#include "smps/smps.h"


Problem Problem::fromSmps(char const *location, GRBEnv &env)
{
    Smps smps;
    smps.readSmps(location);

    return Problem{env};
}
