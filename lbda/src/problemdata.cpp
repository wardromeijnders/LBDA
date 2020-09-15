#include "problemdata.h"

#include "smps/smps.h"

#include <utility>

ProblemData::ProblemData(arma::sp_mat Amat,
                         arma::sp_mat Tmat,
                         arma::sp_mat Wmat,
                         arma::mat scenarios,
                         arma::vec scenarioProbabilities,
                         arma::Col<char> firstStageConstrSenses,
                         arma::Col<char> secondStageConstrSenses,
                         arma::Col<char> firstStageVarTypes,
                         arma::Col<char> secondStageVarTypes,
                         arma::vec firstStageCoeffs,
                         arma::vec secondStageCoeffs,
                         arma::vec firstStageLowerBound,
                         arma::vec firstStageUpperBound,
                         arma::vec secondStageLowerBound,
                         arma::vec secondStageUpperBound,
                         arma::vec firstStageRhs) :
    d_Amat(std::move(Amat)),
    d_Tmat(std::move(Tmat)),
    d_Wmat(std::move(Wmat)),
    d_scenarios(std::move(scenarios)),
    d_scenarioProbabilities(std::move(scenarioProbabilities)),
    d_firstStageConstrSenses(std::move(firstStageConstrSenses)),
    d_secondStageConstrSenses(std::move(secondStageConstrSenses)),
    d_firstStageVarTypes(std::move(firstStageVarTypes)),
    d_secondStageVarTypes(std::move(secondStageVarTypes)),
    d_firstStageCoeffs(std::move(firstStageCoeffs)),
    d_secondStageCoeffs(std::move(secondStageCoeffs)),
    d_firstStageLowerBound(std::move(firstStageLowerBound)),
    d_firstStageUpperBound(std::move(firstStageUpperBound)),
    d_secondStageLowerBound(std::move(secondStageLowerBound)),
    d_secondStageUpperBound(std::move(secondStageUpperBound)),
    d_firstStageRhs(std::move(firstStageRhs))
{
}

ProblemData ProblemData::fromSmps(std::string const &location)
{
    smps::Smps smps;
    smps.read(location);

    return ProblemData{smps.Amat(),
                       smps.Tmat(),
                       smps.Wmat(),
                       smps.generateScenarios(),
                       smps.scenarioProbabilities(),
                       smps.firstStageConstrSenses(),
                       smps.secondStageConstrSenses(),
                       smps.firstStageVarTypes(),
                       smps.secondStageVarTypes(),
                       smps.firstStageObjCoeffs(),
                       smps.secondStageObjCoeffs(),
                       smps.firstStageLowerBound(),
                       smps.firstStageUpperBound(),
                       smps.secondStageLowerBound(),
                       smps.secondStageUpperBound(),
                       smps.firstStageRhs()};
}
