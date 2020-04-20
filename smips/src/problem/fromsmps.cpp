#include "SmiScnModel.hpp"
#include "problem.h"

#include <cassert>

// TODO proper exceptions and handling
Problem Problem::fromSmps(char const *location, GRBEnv &env)
{
    SmiScnModel smi;

    if (smi.readSmps(location) < 0)
        throw "Failed to read file.";

    SmiCoreData *core = smi.getCore();

    if (core->getNumStages() > 2)
        throw "Does not support multi-stage models.";

    assert(core->getNumStages() == 2);
    assert(smi.getNumScenarios() > 0);

    // TODO

    return Problem{env};
}
