#include "smps/smps.h"

#include "smps/coreparser.h"
#include "smps/stochparser.h"
#include "smps/timeparser.h"

#include <gurobi_c++.h>

using namespace smps;

void Smps::readSmps(std::string const &location)
{
    CoreParser coreParser(*this);
    coreParser.parse(location + ".cor");

    TimeParser timeParser(*this);
    timeParser.parse(location + ".tim");

    StochParser stochParser(*this);
    stochParser.parse(location + ".sto");
}

std::string const &Smps::name() const
{
    return d_name;
}

bool Smps::setName(std::string const &name)
{
    d_name = name;
    return true;
}

bool Smps::addObjective(std::string const &name)
{
    if (d_objName.empty())
        d_objName = name;
    else
        return false;  // we already have an objective.

    return true;
}

bool Smps::addConstr(std::string const &name, char type)
{
    d_core.resize(d_core.n_rows + 1, d_core.n_cols);

    d_rhs.resize(d_core.n_rows);
    d_rhs(d_core.n_rows - 1) = 0;

    d_constrSenses.resize(d_core.n_rows);
    d_constrSenses(d_core.n_rows - 1) = type;

    d_constr2idx[name] = d_core.n_rows - 1;

    return true;
}

bool Smps::addCoeff(std::string const &constr,
                    std::string const &var,
                    double coeff,
                    char varType)
{
    if (!d_var2idx.contains(var))  // a new variable.
    {
        d_core.resize(d_core.n_rows, d_core.n_cols + 1);
        d_var2idx[var] = d_core.n_cols - 1;

        d_objCoeffs.resize(d_core.n_cols);
        d_objCoeffs(d_core.n_cols - 1) = 0;

        d_varTypes.resize(d_core.n_cols);
        d_varTypes(d_core.n_cols - 1) = varType;
    }

    if (constr == d_objName)  // is an objective coefficient.
        d_objCoeffs(d_var2idx[var]) = coeff;
    else if (d_constr2idx.contains(constr))  // is an existing constraint.
        d_core(d_constr2idx[constr], d_var2idx[var]) = coeff;
    else
        return false;  // is another free row (but we already have an objective)

    return true;
}

bool Smps::addRhs(std::string const &constr, double coeff)
{
    if (!d_constr2idx.contains(constr))
        return false;

    d_rhs[d_constr2idx[constr]] = coeff;

    return true;
}

bool Smps::addStage(std::string const &constr, std::string const &var)
{
    if (!d_constr2idx.contains(constr) || !d_var2idx.contains(var))
        return false;

    d_stageOffsets.resize(d_stageOffsets.n_rows + 1, 2);

    d_stageOffsets(d_stageOffsets.n_rows - 1, 0) = d_constr2idx[constr];
    d_stageOffsets(d_stageOffsets.n_rows - 1, 1) = d_var2idx[var];

    return true;
}

arma::sp_mat Smps::Amat()
{
    // Of the entire core tableau, the A matrix (first stage constraints on x)
    // is given by the top-left, first stage matrix.
    auto const rowSpan = arma::span(0, d_stageOffsets(1, 0) - 1);
    auto const colSpan = arma::span(0, d_stageOffsets(1, 1) - 1);

    return d_core.submat(rowSpan, colSpan).t();
}

arma::sp_mat Smps::Tmat()
{
    // Given by the bottom-left matrix. These are the second-stage constraint
    // coefficients on the first-stage variables x.
    auto const rowSpan = arma::span(d_stageOffsets(1, 0), d_core.n_rows - 1);
    auto const colSpan = arma::span(0, d_stageOffsets(1, 1) - 1);

    return d_core.submat(rowSpan, colSpan).t();
}

arma::sp_mat Smps::Wmat()
{
    // Given by the bottom-right matrix. These are the second-stage constraint
    // coefficients on the second-stage variables y.
    auto const rowSpan = arma::span(d_stageOffsets(1, 0), d_core.n_rows - 1);
    auto const colSpan = arma::span(d_stageOffsets(1, 1), d_core.n_cols - 1);

    return d_core.submat(rowSpan, colSpan).t();
}

bool Smps::addLowerBound(std::string const &var, double bound)
{
    if (d_lowerBounds.size() != d_core.n_cols)  // not initialised before
        d_lowerBounds = arma::zeros(d_core.n_cols);

    d_lowerBounds[d_var2idx[var]] = bound;
    return true;
}

bool Smps::addUpperBound(std::string const &var, double bound)
{
    if (d_upperBounds.size() != d_core.n_cols)  // not initialised before
        d_upperBounds = arma::vec(d_core.n_cols).fill(arma::datum::inf);

    d_upperBounds[d_var2idx[var]] = bound;
    return true;
}

bool Smps::addIndep(std::string const &constr, std::pair<double, double> value)
{
    d_indep[d_constr2idx[constr]].emplace_back(value);
    return true;
}

bool Smps::addScenario(std::string const &scenario,
                       std::string const &parent,
                       double probability)
{
    return true;  // TODO
}

bool Smps::addScenarioRealisation(std::string const &scenario,
                                  std::string const &constr,
                                  double value)
{
    return true;  // TODO
}


arma::vec Smps::firstStageObjCoeffs()
{
    return d_objCoeffs.subvec(0, d_stageOffsets(1, 1) - 1);
}

bool Smps::addVarType(std::string const &var, char type)
{
    d_varTypes(d_var2idx[var]) = type;
}

arma::vec Smps::secondStageObjCoeffs()
{
    return d_objCoeffs.subvec(d_stageOffsets(1, 1), d_objCoeffs.size() - 1);
}

arma::Col<char> Smps::firstStageConstrSenses()
{
    return d_constrSenses.subvec(0, d_stageOffsets(1, 0) - 1);
}

arma::Col<char> Smps::secondStageConstrSenses()
{
    return d_constrSenses.subvec(d_stageOffsets(1, 0),
                                 d_constrSenses.size() - 1);
}

arma::Col<char> Smps::firstStageVarTypes()
{
    return d_varTypes.subvec(0, d_stageOffsets(1, 1) - 1);
}

arma::Col<char> Smps::secondStageVarTypes()
{
    return d_varTypes.subvec(d_stageOffsets(1, 1), d_varTypes.size() - 1);
}

arma::vec Smps::firstStageLowerBound()
{
    if (d_lowerBounds.size() != d_core.n_cols)
        d_lowerBounds = arma::zeros(d_core.n_cols);

    return d_lowerBounds.subvec(0, d_stageOffsets(1, 1) - 1);
}

arma::vec Smps::firstStageUpperBound()
{
    if (d_upperBounds.size() != d_core.n_cols)
        d_upperBounds = arma::vec(d_core.n_cols).fill(arma::datum::inf);

    return d_upperBounds.subvec(0, d_stageOffsets(1, 1) - 1);
}

arma::vec Smps::secondStageLowerBound()
{
    if (d_lowerBounds.size() != d_core.n_cols)
        d_lowerBounds = arma::zeros(d_core.n_cols);

    return d_lowerBounds.subvec(d_stageOffsets(1, 1), d_lowerBounds.size() - 1);
}

arma::vec Smps::secondStageUpperBound()
{
    if (d_upperBounds.size() != d_core.n_cols)
        d_upperBounds = arma::vec(d_core.n_cols).fill(arma::datum::inf);

    return d_upperBounds.subvec(d_stageOffsets(1, 1), d_upperBounds.size() - 1);
}

arma::vec Smps::firstStageRhs()
{
    return d_rhs.subvec(0, d_stageOffsets(1, 0) - 1);
}

arma::mat Smps::generateScenarios()
{
    if (d_indep.size() > 0)
        return genIndepScenarios();

    return arma::mat();  // TODO
}

arma::vec Smps::scenarioProbabilities()
{
    if (d_indep.size() > 0)
        return indepScenProbabilities();

    return arma::vec();  // TODO
}

arma::mat Smps::genIndepScenarios()
{
    // This is loosely based on https://stackoverflow.com/a/48271759/4316405.
    size_t nScenarios = 1;

    for (auto const &[idx, distr] : d_indep)
        nScenarios *= distr.size();

    // Second-stage RHS (rows) for each scenario (cols).
    arma::mat scenarios(d_core.n_rows - d_stageOffsets(1, 0), nScenarios);

    for (size_t scenario = 0; scenario != nScenarios; ++scenario)
    {
        auto temp = scenario;

        for (auto const &[constr, vec] : d_indep)
        {
            auto index = temp % vec.size();
            temp /= vec.size();
            scenarios(constr - d_stageOffsets(1, 0), scenario) = vec[index].first;
        }
    }

    return scenarios;
}

arma::vec Smps::indepScenProbabilities()
{
    size_t nScenarios = 1;

    for (auto const &[idx, distr] : d_indep)
        nScenarios *= distr.size();

    arma::vec probabilities = arma::ones(nScenarios);

    // TODO this partially duplicates genIndepScenarios above.
    for (size_t scenario = 0; scenario != nScenarios; ++scenario)
    {
        auto temp = scenario;

        for (auto const &[constr, vec] : d_indep)
        {
            auto index = temp % vec.size();
            temp /= vec.size();

            probabilities(scenario) *= vec[index].second;
        }
    }

    return probabilities;
}
