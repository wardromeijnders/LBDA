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
    auto const &firstDataName = dataLine.firstDataName();
    auto const &secondDataName = dataLine.secondDataName();

    if (firstDataName.find("MARKER") != std::string::npos)
    {
        if (secondDataName.find("INTORG") != std::string::npos)
            d_parseInts = true;

        if (secondDataName.find("INTEND") != std::string::npos)
            d_parseInts = false;

        return true;  // this is a marker line, nothing to parse.
    }

    auto const &var = dataLine.name();
    auto const &[constr1, coeff1] = dataLine.firstDataEntry();
    auto type = d_parseInts ? GRB_INTEGER : GRB_CONTINUOUS;

    auto res = d_smps.addCoeff(constr1, var, coeff1, type);

    if (!dataLine.hasSecondDataEntry())
        return res;

    auto const &[constr2, coeff2] = dataLine.secondDataEntry();
    return res && d_smps.addCoeff(constr2, var, coeff2, type);
}

bool CoreParser::parseRhs(DataLine const &dataLine)
{
    auto [constr1, coeff1] = dataLine.firstDataEntry();
    auto res = d_smps.addRhs(constr1, coeff1);

    if (!dataLine.hasSecondDataEntry())
        return res;

    auto [constr2, coeff2] = dataLine.secondDataEntry();
    return res && d_smps.addRhs(constr2, coeff2);
}

bool CoreParser::parseBounds(DataLine const &dataLine)
{
    auto [var, bound] = dataLine.firstDataEntry();

    // This follows mostly from
    // http://www-eio.upc.es/lceio/manuals/cplex-11/html/reffileformatscplex/reffileformatscplex11.html
    if (dataLine.indicator() == "LO")  // lower bound
        return d_smps.addLowerBound(var, bound);

    if (dataLine.indicator() == "UP")  // upper bound
        return d_smps.addUpperBound(var, bound);

    if (dataLine.indicator() == "LI")  // integer lower bound
        return d_smps.addVarType(var, GRB_INTEGER)
               && d_smps.addLowerBound(var, bound);

    if (dataLine.indicator() == "UI")  // integer upper bound
        return d_smps.addVarType(var, GRB_INTEGER)
               && d_smps.addUpperBound(var, bound);

    if (dataLine.indicator() == "BV")  // binary variable
        return d_smps.addVarType(var, GRB_BINARY)
               && d_smps.addLowerBound(var, 0.0)
               && d_smps.addUpperBound(var, 1.0);

    return false;
}

bool CoreParser::parseRanges(DataLine const &dataLine)
{
    std::cerr << "SMIPS does not currently understand RANGES.\n";
    return false;
}
