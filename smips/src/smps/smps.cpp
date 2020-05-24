#include "smps/smps.h"

#include "smps/core/nonestate.h"

#include <memory>

bool Smps::readSmps(std::string const &location)
{
    readCoreFile(location + ".cor");
    readStochFile(location + ".sto");
    readTimeFile(location + ".tim");

    return isValid();
}

void Smps::readStochFile(std::string const &location)
{
    // TODO
}

void Smps::readTimeFile(std::string const &location)
{
    // TODO
}

void Smps::readCoreFile(std::string const &location)
{
    std::ifstream file(location);
    std::string line;

    std::unique_ptr<ParserState> state = std::make_unique<NoneState>();

    while (std::getline(file, line))
    {
        if (line.starts_with('*'))  // comment
            continue;

        if (state->maybeTransition(state, line))  // header row
            continue;

        if (!state->parse(*this, line))  // could not parse - why?
            std::cerr << line << '\n';
    }
}

bool Smps::isValid()
{
    // TODO
    return true;
}

std::string Smps::trim(std::string const &target)
{
    size_t first = target.find_first_not_of(' ');

    if (first == std::string::npos)
        return target;

    size_t last = target.find_last_not_of(' ');

    return target.substr(first, last - first + 1);
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
    d_objName = trim(name);
}

void Smps::addConstr(std::string const &name, char type)
{
    d_core.resize(d_core.n_rows + 1, d_core.n_cols);

    d_rhs.resize(d_core.n_rows);
    d_rhs(d_core.n_rows - 1) = 0;

    d_row2idx[trim(name)] = d_core.n_rows - 1;

    // TODO type
}

void Smps::addCoeff(std::string const &constr,
                    std::string const &var,
                    double coeff)
{
    std::string const cleanVar = trim(var);
    std::string const cleanConstr = trim(constr);

    if (!d_col2idx.contains(cleanVar))
    {
        d_core.resize(d_core.n_rows, d_core.n_cols + 1);
        d_col2idx[cleanVar] = d_core.n_cols - 1;
    }

    d_core(d_row2idx[cleanConstr], d_col2idx[cleanVar]) = coeff;
}

void Smps::addRhs(std::string const &constr, double coeff)
{
    std::string cleanConstr = trim(constr);
    d_rhs[d_row2idx[cleanConstr]] = coeff;
}
