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

    size_t nStages = core->getNumStages();
    size_t nScenarios = smi.getNumScenarios();

    if (nStages > 2)
        throw "Does not support multi-stage models.";

    assert(nStages == 2);
    assert(nScenarios > 0);

    // TODO rename these once I have a better understanding of their contextual
    //  meaning.
    auto nRows = arma::Col<int>(nStages);
    auto nCols = arma::Col<int>(nStages);
    auto nInts = arma::Col<int>(nStages);
    auto rStart = arma::Col<int>(nStages);
    auto cStart = arma::Col<int>(nStages);

    for (size_t stage = 0; stage != nStages; ++stage)
    {
        nRows[stage] = core->getNumRows(stage);
        nCols[stage] = core->getNumCols(stage);
        nInts[stage] = core->getIntCols(stage).size();

        rStart[stage] = core->getRowStart(stage);
        cStart[stage] = core->getColStart(stage);

        // TODO core data
    }

    auto probs = arma::vec(nScenarios);

    for (size_t scenario = 0; scenario != nScenarios; ++scenario)
    {
        auto scnNode = smi.getLeafNode(scenario);
        probs[scenario] = scnNode->getProb();

        // TODO scenario data
    }

    return Problem{env};
}
