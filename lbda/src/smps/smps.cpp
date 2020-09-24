#include "smps/smps.h"

#include "smps/coreparser.h"
#include "smps/stochparser.h"
#include "smps/timeparser.h"


using namespace smps;

void Smps::read(std::string const &location)
{
    CoreParser coreParser(*this);
    coreParser.parse(location + ".cor");

    TimeParser timeParser(*this);
    timeParser.parse(location + ".tim");

    StochParser stochParser(*this);
    stochParser.parse(location + ".sto");
}

void Smps::addObjective(std::string const &name)
{
    if (d_objName.empty())
        d_objName = name;
}

void Smps::addConstr(std::string const &name, char type)
{
    d_core.resize(d_core.n_rows + 1, d_core.n_cols);

    d_rhs.resize(d_core.n_rows);
    d_rhs(d_core.n_rows - 1) = 0;

    d_constrSenses.resize(d_core.n_rows);
    d_constrSenses(d_core.n_rows - 1) = type;

    d_constr2idx[name] = d_core.n_rows - 1;
}

void Smps::addCoeff(std::string const &constr,
                    std::string const &var,
                    double coeff,
                    char varType)
{
    if (!d_var2idx.count(var))  // a new variable.
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
    else if (d_constr2idx.count(constr))  // is an existing constraint.
        d_core(d_constr2idx[constr], d_var2idx[var]) = coeff;
}

void Smps::addRhs(std::string const &constr, double coeff)
{
    if (d_constr2idx.count(constr))
        d_rhs[d_constr2idx[constr]] = coeff;
}

void Smps::addStage(std::string const &constr, std::string const &var)
{
    if (!d_constr2idx.count(constr) || !d_var2idx.count(var))
        return;

    d_stageOffsets.resize(d_stageOffsets.n_rows + 1, 2);

    d_stageOffsets(d_stageOffsets.n_rows - 1, 0) = d_constr2idx[constr];
    d_stageOffsets(d_stageOffsets.n_rows - 1, 1) = d_var2idx[var];
}

arma::sp_mat Smps::Amat()
{
    // Of the entire core tableau, the A matrix (first stage constraints on x)
    // is given by the top-left, first stage matrix.
    auto const rowEnd = std::max(d_stageOffsets(1, 0), 1ULL) - 1;
    auto const colEnd = std::max(d_stageOffsets(1, 1), 1ULL) - 1;

    // Starts in the same row (constraint), so the first stage is empty
    if (d_stageOffsets(1, 0) == d_stageOffsets(0, 0))
        return arma::sp_mat();

    return d_core.submat(arma::span(0, rowEnd), arma::span(0, colEnd)).t();
}

arma::sp_mat Smps::Tmat()
{
    // Given by the bottom-left matrix. These are the second-stage constraint
    // coefficients on the first-stage variables x.
    auto const rowEnd = std::max(d_core.n_rows, 1ULL) - 1;
    auto const colEnd = std::max(d_stageOffsets(1, 1), 1ULL) - 1;

    auto const rowSpan = arma::span(d_stageOffsets(1, 0), rowEnd);
    auto const colSpan = arma::span(0, colEnd);

    return d_core.submat(rowSpan, colSpan).t();
}

arma::sp_mat Smps::Wmat()
{
    // Given by the bottom-right matrix. These are the second-stage constraint
    // coefficients on the second-stage variables y.
    auto const rowEnd = std::max(d_core.n_rows, 1ULL) - 1;
    auto const colEnd = std::max(d_core.n_cols, 1ULL) - 1;

    auto const rowSpan = arma::span(d_stageOffsets(1, 0), rowEnd);
    auto const colSpan = arma::span(d_stageOffsets(1, 1), colEnd);

    return d_core.submat(rowSpan, colSpan).t();
}

void Smps::addLowerBound(std::string const &var, double bound)
{
    if (d_lowerBounds.size() != d_core.n_cols)  // not initialised before
        d_lowerBounds = arma::zeros(d_core.n_cols);

    d_lowerBounds[d_var2idx[var]] = bound;
}

void Smps::addUpperBound(std::string const &var, double bound)
{
    if (d_upperBounds.size() != d_core.n_cols)  // not initialised before
        d_upperBounds = arma::vec(d_core.n_cols).fill(arma::datum::inf);

    d_upperBounds[d_var2idx[var]] = bound;
}

void Smps::addIndep(std::string const &constr, std::pair<double, double> value)
{
    d_indep[d_constr2idx[constr]].emplace_back(value);
}

void Smps::addScenario(std::string const &scenario, double probability)
{
    size_t const idx = d_scenarios.size();
    ScenNode scen{probability, {}};

    d_scenarios.push_back(scen);
    d_scen2idx[scenario] = idx;
}

void Smps::addScenarioRealisation(std::string const &scenario,
                                  std::string const &constr,
                                  double value)
{
    if (!d_scen2idx.count(scenario) || !d_constr2idx.count(constr))
        return;

    size_t const idx = d_scen2idx[scenario];
    size_t const constrIdx = d_constr2idx[constr];

    d_scenarios[idx].rhs[constrIdx] = value;
}

arma::vec Smps::firstStageObjCoeffs()
{
    auto const vecEnd = std::max(d_stageOffsets(1, 1), 1ULL) - 1;
    return d_objCoeffs.subvec(0, vecEnd);
}

arma::vec Smps::secondStageObjCoeffs()
{
    auto const vecEnd = std::max(d_objCoeffs.size(), 1ULL) - 1;
    return d_objCoeffs.subvec(d_stageOffsets(1, 1), vecEnd);
}

arma::Col<char> Smps::firstStageConstrSenses()
{
    auto const vecEnd = std::max(d_stageOffsets(1, 0), 1ULL) - 1;
    return d_constrSenses.subvec(0, vecEnd);
}

arma::Col<char> Smps::secondStageConstrSenses()
{
    auto const vecEnd = std::max(d_constrSenses.size(), 1ULL) - 1;
    return d_constrSenses.subvec(d_stageOffsets(1, 0), vecEnd);
}

arma::Col<char> Smps::firstStageVarTypes()
{
    auto const vecEnd = std::max(d_stageOffsets(1, 1), 1ULL) - 1;
    return d_varTypes.subvec(0, vecEnd);
}

arma::Col<char> Smps::secondStageVarTypes()
{
    auto const vecEnd = std::max(d_varTypes.size(), 1ULL) - 1;
    return d_varTypes.subvec(d_stageOffsets(1, 1), vecEnd);
}

arma::vec Smps::firstStageLowerBound()
{
    if (d_lowerBounds.size() != d_core.n_cols)
        d_lowerBounds = arma::zeros(d_core.n_cols);

    auto const vecEnd = std::max(d_stageOffsets(1, 1), 1ULL) - 1;
    return d_lowerBounds.subvec(0, vecEnd);
}

arma::vec Smps::firstStageUpperBound()
{
    if (d_upperBounds.size() != d_core.n_cols)
        d_upperBounds = arma::vec(d_core.n_cols).fill(arma::datum::inf);

    auto const vecEnd = std::max(d_stageOffsets(1, 1), 1ULL) - 1;
    return d_upperBounds.subvec(0, vecEnd);
}

arma::vec Smps::secondStageLowerBound()
{
    if (d_lowerBounds.size() != d_core.n_cols)
        d_lowerBounds = arma::zeros(d_core.n_cols);

    auto const vecEnd = std::max(d_lowerBounds.size(), 1ULL) - 1;
    return d_lowerBounds.subvec(d_stageOffsets(1, 1), vecEnd);
}

arma::vec Smps::secondStageUpperBound()
{
    if (d_upperBounds.size() != d_core.n_cols)
        d_upperBounds = arma::vec(d_core.n_cols).fill(arma::datum::inf);

    auto const vecEnd = std::max(d_upperBounds.size(), 1ULL) - 1;
    return d_upperBounds.subvec(d_stageOffsets(1, 1), vecEnd);
}

arma::vec Smps::firstStageRhs()
{
    auto const vecEnd = std::max(d_stageOffsets(1, 0), 1ULL) - 1;
    return d_rhs.subvec(0, vecEnd);
}

arma::vec Smps::secondStageRhs()
{
    auto const vecEnd = std::max(d_rhs.size(), 1ULL) - 1;
    return d_rhs.subvec(d_stageOffsets(1, 0), vecEnd);
}

arma::mat Smps::generateScenarios()
{
    if (!d_indep.empty())
        return genIndepScenarios();

    if (!d_scenarios.empty())
        return genScenarios();

    throw std::runtime_error("LBDA+ does not understand this type of "
                             ".sto file yet.");
}

arma::vec Smps::scenarioProbabilities()
{
    if (!d_indep.empty())
        return indepScenProbabilities();

    if (!d_scenarios.empty())
        return scenProbabilities();

    throw std::runtime_error("LBDA+ does not understand this type of "
                             ".sto file yet.");
}

arma::mat Smps::genIndepScenarios()
{
    // This is loosely based on https://stackoverflow.com/a/48271759/4316405.
    size_t nScenarios = 1;

    for (auto const &[idx, distr] : d_indep)
        nScenarios *= distr.size();

    // Second-stage RHS (rows) for each scenario (cols). The base RHS is
    // replicated and overwritten with the scenario-specific RHS, where required.
    arma::mat scenarios = arma::repmat(secondStageRhs(), 1, nScenarios);

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

arma::mat Smps::genScenarios()
{
    // Second-stage RHS (rows) for each scenario (cols). The base RHS is
    // replicated and overwritten with the scenario-specific RHS, where required.
    arma::mat scenarios = arma::repmat(secondStageRhs(), 1, d_scenarios.size());

    for (size_t scenIdx = 0; scenIdx != d_scenarios.size(); ++scenIdx)
        for (auto const &[constr, value] : d_scenarios[scenIdx].rhs)
            scenarios(constr - d_stageOffsets(1, 0), scenIdx) = value;

    return scenarios;
}

arma::vec Smps::scenProbabilities()
{
    arma::vec probabilities(d_scenarios.size());

    for (size_t scenIdx = 0; scenIdx != d_scenarios.size(); ++scenIdx)
        probabilities[scenIdx] = d_scenarios[scenIdx].probability;

    return probabilities;
}
