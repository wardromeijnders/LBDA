#include "smps/core/rhsstate.h"

#include "smps/core/endatastate.h"

#include <cassert>

bool RhsState::parse(Smps &smps, std::string &line)
{
    std::string constr(line.substr(14, 7));
    double value = std::stod(line.substr(24, 11));

    smps.addRhs(constr, value);

    return true;
}

bool RhsState::maybeTransition(std::unique_ptr<ParserState> &state,
                               std::string &line)
{
    // TODO allow BOUNDS/RANGES?

    if (line.starts_with("ENDATA"))
    {
        state = std::make_unique<EndataState>();
        return true;
    }

    return false;
}
