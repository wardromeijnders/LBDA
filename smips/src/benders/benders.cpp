#include "benders.h"

Benders::Benders(MasterProblem &master) :
    d_nCuts(0),
    d_runTime(0),
    d_master(master)
{
}
