#include "smps/core/rowstate.h"

#include "smps/core/colstate.h"

#include <gurobi_c++.h>

using namespace smps::core;

bool RowState::parse(smps::Smps &smps, std::string &line)
{
    std::map<char, char> senses = {{'L', GRB_LESS_EQUAL},
                                   {'E', GRB_EQUAL},
                                   {'G', GRB_GREATER_EQUAL}};

    char type = line[1];

    if (std::string("NLEG").find(type) == std::string::npos)
        return false;

    std::string row(line.substr(4, 7));

    if (type == 'N')  // "Free" row, which is generally the objective.
        smps.addObjective(row);
    else
        smps.addConstr(row, senses[type]);

    return true;
}

bool RowState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                               std::string &line)
{
    if (line.starts_with("COLUMNS"))
    {
        state = std::make_unique<ColState>();
        return true;
    }

    return false;
}
