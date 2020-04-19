#include "problem.h"

#include <coin/SmiScnModel.hpp>


// TODO exception handling
Problem Problem::fromSmps(char const *location)
{
    SmiScnModel smi;

    int status = smi.readSmps(location);

    if (status < 0)
        throw "Failed to read file.";

    SmiCoreData *core = smi.getCore();

    if (core->getNumStages() > 2)
        throw "Does not support multi-stage models.";



    // TODO
}
