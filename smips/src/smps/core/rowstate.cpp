#include "smps/core/rowstate.h"
#include "smps/core/colstate.h"

#include <cassert>

bool RowState::parse(Smps &smps, std::string &line)
{
    char type = line[1];
    assert(type == 'N' || type == 'L' || type == 'E' || type == 'G');

    std::string row(line.substr(4, 7));

    if (type == 'N') // "Free", sets the objective (as a matrix row).
        smps.addObjective(row);

    smps.addConstr(row, type);

    return true;
}

bool RowState::maybeTransition(std::unique_ptr<ParserState> &state,
                               std::string &line)
{
    if (line.starts_with("COLUMNS"))
    {
        state = std::make_unique<ColState>();
        return true;
    }

    return false;
}
