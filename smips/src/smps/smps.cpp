#include "smps/smps.h"

#include "smps/core/nonestate.h"
#include "smps/time/nonestate.h"

#include <memory>

using namespace smps;

void Smps::readSmps(std::string const &location)
{
    // TODO deduplicate template methods
    readCoreFile(location + ".cor");
    readStochFile(location + ".sto");
    readTimeFile(location + ".tim");
}

void Smps::readStochFile(std::string const &location)
{
    // TODO
}

void Smps::readTimeFile(std::string const &location)
{
    std::ifstream file(location);
    std::string line;

    std::unique_ptr<ParserState> state = std::make_unique<time::NoneState>();

    while (std::getline(file, line))
    {
        if (line.starts_with('*'))  // comment
            continue;

        if (state->maybeTransition(state, line))  // header row
            continue;

        if (!state->parse(*this, line))  // could not parse - why?
            std::cerr << "Failed parsing: " << line << '\n';
    }
}

void Smps::readCoreFile(std::string const &location)
{
    std::ifstream file(location);
    std::string line;

    std::unique_ptr<ParserState> state = std::make_unique<core::NoneState>();

    while (std::getline(file, line))
    {
        if (line.starts_with('*'))  // comment
            continue;

        if (state->maybeTransition(state, line))  // header row
            continue;

        if (!state->parse(*this, line))  // could not parse - why?
            std::cerr << "Failed parsing: " << line << '\n';
    }
}

std::string const &Smps::name() const
{
    return d_name;
}

void Smps::setName(std::string &name)
{
    d_name = trim(name);
}

void Smps::addObjective(std::string const &name)
{
    if (d_objName.empty())
        d_objName = trim(name);
}

void Smps::addConstr(std::string const &name, char type)
{
    d_core.resize(d_core.n_rows + 1, d_core.n_cols);

    d_rhs.resize(d_core.n_rows);
    d_rhs(d_core.n_rows - 1) = 0;

    d_constrSenses.resize(d_core.n_rows);
    d_constrSenses(d_core.n_rows - 1) = type;

    d_row2idx[trim(name)] = d_core.n_rows - 1;
}

void Smps::addCoeff(std::string const &constr,
                    std::string const &var,
                    double coeff)
{
    std::string const cleanVar = trim(var);
    std::string const cleanConstr = trim(constr);

    if (!d_col2idx.contains(cleanVar))  // a new variable.
    {
        d_core.resize(d_core.n_rows, d_core.n_cols + 1);
        d_col2idx[cleanVar] = d_core.n_cols - 1;

        d_objCoeffs.resize(d_core.n_cols);
        d_objCoeffs(d_core.n_cols - 1) = 0;
    }

    if (cleanConstr == d_objName)  // is an objective coefficient.
        d_objCoeffs(d_col2idx[cleanVar]) = coeff;
    else if (d_row2idx.contains(cleanConstr))  // is an existing constraint.
        d_core(d_row2idx[cleanConstr], d_col2idx[cleanVar]) = coeff;
}

void Smps::addRhs(std::string const &constr, double coeff)
{
    std::string cleanConstr = trim(constr);
    d_rhs[d_row2idx[cleanConstr]] = coeff;
}

std::string smps::trim(std::string const &target)
{
    size_t first = target.find_first_not_of(' ');

    if (first == std::string::npos)
        return target;

    size_t last = target.find_last_not_of(' ');

    return target.substr(first, last - first + 1);
}
