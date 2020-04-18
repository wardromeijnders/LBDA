#include "deterministicequivalent.h"

DeterministicEquivalent::DeterministicEquivalent(GRBEnv &env,
                                                 Problem const &problem) :
    d_problem(problem),
    d_model(GRBModel(env)),
    d_status(status::UNSOLVED)
{
    initFirstStage();
    initSecondStage();
}
