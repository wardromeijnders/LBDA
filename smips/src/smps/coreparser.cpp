#include "smps/coreparser.h"

#include <gurobi_c++.h>
#include <iterator>

using namespace smps;

bool (CoreParser::*(CoreParser::d_actions)[])(smps::DataLine const &)
    = {&CoreParser::parseNone,
       &CoreParser::parseName,
       &CoreParser::parseRows,
       &CoreParser::parseCols,
       &CoreParser::parseRhs,
       &CoreParser::parseBounds,
       &CoreParser::parseRanges,
       &CoreParser::parseEndata};

void CoreParser::parse(std::string const &location)
{
    std::ifstream file(location);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.starts_with('*'))  // comment
            continue;

        if (transition(line))  // header row
            continue;

        DataLine const dataLine(line);

        if (!std::invoke(d_actions[d_state], this, dataLine))  // bad parse
            std::cerr << "Failed parsing: " << line << '\n';
    }
}

bool CoreParser::transition(std::string const &line)
{
    std::string names[] = {"NONE",
                           "NAME",
                           "ROWS",
                           "COLUMNS",
                           "RHS",
                           "BOUNDS",
                           "RANGES",
                           "ENDATA"};

    for (size_t idx = 0; idx != std::size(names); ++idx)
        if (line.starts_with(names[idx]))
        {
            d_state = static_cast<State>(idx);
            return true;
        }

    return false;
}

bool CoreParser::parseName(DataLine const &dataLine)
{
    return d_smps.setName(dataLine.firstDataName());
}

bool CoreParser::parseRows(DataLine const &dataLine)
{
    std::map<char, char> senses = {{'L', GRB_LESS_EQUAL},
                                   {'E', GRB_EQUAL},
                                   {'G', GRB_GREATER_EQUAL}};

    char type = dataLine.indicator()[0];

    if (std::string("NLEG").find(type) == std::string::npos)
        return false;

    return type == 'N'  // "free" row, which is generally the objective.
               ? d_smps.addObjective(dataLine.name())
               : d_smps.addConstr(dataLine.name(), senses[type]);
}

bool CoreParser::parseCols(DataLine const &dataLine)
{
    auto [constr, coeff] = dataLine.firstDataEntry();
    auto res = d_smps.addCoeff(constr, dataLine.name(), coeff);

    if (!dataLine.hasSecondDataEntry())
        return res;

    // TODO second data entry?

    return res;
}

bool CoreParser::parseRhs(DataLine const &dataLine)
{
    auto [constr, coeff] = dataLine.firstDataEntry();
    return d_smps.addRhs(constr, coeff);
}

bool CoreParser::parseBounds(DataLine const &dataLine)
{
    // TODO check if this works.
    auto [var, bound] = dataLine.firstDataEntry();

    if (dataLine.indicator() == "LO")  // lower bound
        return d_smps.addLowerBound(var, bound);

    if (dataLine.indicator() == "UP")  // upper bound
        return d_smps.addUpperBound(var, bound);

    return false;
}

bool CoreParser::parseRanges(DataLine const &dataLine)
{
    return false;  // TODO
}
