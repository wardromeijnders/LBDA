#include "problem.h"

#include "smps/smps.h"

Problem Problem::fromSmps(std::string const &location)
{
    smps::Smps smps;
    smps.readSmps(location);

    Problem problem{};

    problem.d_Amat = smps.Amat();
    problem.d_Wmat = smps.Wmat();
    problem.d_Tmat = smps.Tmat();

    problem.d_firstStageCoeffs = smps.firstStageObjCoeffs();
    problem.d_secondStageCoeffs = smps.secondStageObjCoeffs();

    problem.d_firstStageConstrSenses = smps.firstStageConstrSenses();
    problem.d_secondStageConstrSenses = smps.secondStageConstrSenses();

    problem.d_firstStageVarTypes = smps.firstStageVarTypes();
    problem.d_secondStageVarTypes = smps.secondStageVarTypes();

    problem.d_firstStageLowerBound = smps.firstStageLowerBound();
    problem.d_firstStageUpperBound = smps.firstStageUpperBound();
    problem.d_secondStageLowerBound = smps.secondStageLowerBound();
    problem.d_secondStageUpperBound = smps.secondStageUpperBound();

    problem.d_firstStageRhs = smps.firstStageRhs();

    problem.d_scenarios = smps.generateScenarios();
    problem.d_scenarioProbabilities = smps.scenarioProbabilities();

    return problem;
}
