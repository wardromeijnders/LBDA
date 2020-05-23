#include "SmiScnModel.hpp"
#include "problem.h"

#include <cassert>

// TODO proper exceptions and handling
Problem Problem::fromSmps(char const *location, GRBEnv &env)
{
    SmiScnModel smi;

    // TODO this prints SMPS file information. Do we want to keep that?
    if (smi.readSmps(location) < 0)
        throw "Failed to read file.";

    SmiCoreData *core = smi.getCore();

    auto nStages = core->getNumStages();
    auto nScenarios = smi.getNumScenarios();

    if (nStages > 2)
        throw "Multi-stage models are not supported.";

    /** allocate memory */
    assert(nStages == 2);
    assert(nScenarios > 0);

    arma::uvec nRows(nStages);
    arma::uvec nCols(nStages);
    arma::uvec nInts(nStages);
    arma::uvec rowStart(nStages);
    arma::uvec colStart(nStages);

    auto clbd_core_ = new double *[nStages];
    auto cubd_core_ = new double *[nStages];
    auto obj_core_ = new double *[nStages];
    auto rlbd_core_ = new double *[nStages];
    auto rubd_core_ = new double *[nStages];

    auto ctype_core_ = new char *[nStages];

    /** stage information */
    auto nrows_core_ = 0;
    auto ncols_core_ = 0;
    auto nints_core_ = 0;

    for (int stage = 0; stage != nStages; ++stage)
    {
        assert(core->getNumRows(stage) >= 0);
        assert(core->getNumCols(stage) >= 0);
        assert(core->getRowStart(stage) >= 0);
        assert(core->getColStart(stage) >= 0);

        nRows[stage] = core->getNumRows(stage);
        nCols[stage] = core->getNumCols(stage);

        nInts[stage] = (int) core->getIntCols(stage).size();
        rowStart[stage] = core->getRowStart(stage);
        colStart[stage] = core->getColStart(stage);

        nrows_core_ += nRows[stage];
        ncols_core_ += nCols[stage];

        clbd_core_[stage] = new double[nCols[stage]];
        cubd_core_[stage] = new double[nCols[stage]];
        obj_core_[stage] = new double[nCols[stage]];
        rlbd_core_[stage] = new double[nRows[stage]];
        rubd_core_[stage] = new double[nRows[stage]];
        ctype_core_[stage] = new char[nCols[stage]];

        core->copyColLower(clbd_core_[stage], stage);
        core->copyColUpper(cubd_core_[stage], stage);
        core->copyObjective(obj_core_[stage], stage);
        core->copyRowLower(rlbd_core_[stage], stage);
        core->copyRowUpper(rubd_core_[stage], stage);

        /** set column types */
        CoinFillN(ctype_core_[stage], nCols[stage], 'C');
        auto binIndices = core->getBinaryIndices();

        for (int j = 0; j < core->getBinaryLength(); ++j)
        {
            if (binIndices[j] < colStart[stage]
                || binIndices[j] >= colStart[stage] + nCols[stage])
                continue;

            ctype_core_[stage][binIndices[j] - colStart[stage]] = 'B';
            nints_core_++;
        }

        auto intIndices = core->getIntegerIndices();
        for (int j = 0; j < core->getIntegerLength(); ++j)
        {
            if (intIndices[j] < colStart[stage]
                || intIndices[j] >= colStart[stage] + nCols[stage])
                continue;

            ctype_core_[stage][intIndices[j] - colStart[stage]] = 'I';
            nints_core_++;
        }
    }

    /** construct core matrix rows */
    int rowIdx = 0;
    auto rows_core_ = new CoinPackedVector *[nrows_core_];

    for (int s = 0; s < nStages; ++s)
    {
        SmiNodeData *node = core->getNode(s);

        for (int i = rowStart[s]; i < rowStart[s] + nRows[s]; ++i)
        {
            if (i != rowIdx)
                throw "Unexpected core structure";

            rows_core_[rowIdx++] = new CoinPackedVector(node->getRowLength(i),
                                                        node->getRowIndices(i),
                                                        node->getRowElements(i));
        }
    }

    assert(rowIdx == nrows_core_);

    auto probs = new double[nScenarios];

    auto clbd_scen_ = new CoinPackedVector *[nScenarios];
    auto cubd_scen_ = new CoinPackedVector *[nScenarios];
    auto obj_scen_ = new CoinPackedVector *[nScenarios];
    auto rlbd_scen_ = new CoinPackedVector *[nScenarios];
    auto rubd_scen_ = new CoinPackedVector *[nScenarios];

    std::cout << nScenarios << '\n';

    for (int scenario = 0; scenario != nScenarios; ++scenario)
    {
        auto const leafNode = smi.getLeafNode(scenario);

        probs[scenario] = leafNode->getProb();

        SmiNodeData *node = leafNode->getNode();

        clbd_scen_[scenario] = new CoinPackedVector(node->getColLowerLength(),
                                                    node->getColLowerIndices(),
                                                    node->getColLowerElements());
        std::cout << "Col LB\n";
        for (int idx = 0; idx != node->getColLowerLength(); ++idx)
            std::cout << (*clbd_scen_[scenario])[idx] << ' ';
        std::cout << '\n';

        cubd_scen_[scenario] = new CoinPackedVector(node->getColUpperLength(),
                                                    node->getColUpperIndices(),
                                                    node->getColUpperElements());
        std::cout << "Col UB\n";
        for (int idx = 0; idx != node->getColUpperLength(); ++idx)
            std::cout << (*cubd_scen_[scenario])[idx] << ' ';
        std::cout << '\n';

        obj_scen_[scenario] = new CoinPackedVector(node->getObjectiveLength(),
                                                   node->getObjectiveIndices(),
                                                   node->getObjectiveElements());
        std::cout << "Obj\n";
        for (int idx = 0; idx != node->getObjectiveLength(); ++idx)
            std::cout << (*obj_scen_[scenario])[idx] << ' ';
        std::cout << '\n';

        rlbd_scen_[scenario] = new CoinPackedVector(node->getRowLowerLength(),
                                                    node->getRowLowerIndices(),
                                                    node->getRowLowerElements());
        std::cout << "Row LB\n";
        for (int idx = 0; idx != node->getRowLowerLength(); ++idx)
            std::cout << (*rlbd_scen_[scenario])[idx] << ' ';
        std::cout << '\n';

        rubd_scen_[scenario] = new CoinPackedVector(node->getRowUpperLength(),
                                                    node->getRowUpperIndices(),
                                                    node->getRowUpperElements());
        std::cout << "Row UB\n";
        for (int idx = 0; idx != node->getRowUpperLength(); ++idx)
            std::cout << (*rubd_scen_[scenario])[idx] << ' ';
        std::cout << '\n';
    }

    return Problem{env};
}
