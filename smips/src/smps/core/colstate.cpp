#include "smps/core/colstate.h"

#include "smps/core/rhsstate.h"


bool ColState::parse(Smps &smps, std::string &line)
{
    std::string var(line.substr(4, 7));
    std::string constr(line.substr(14, 7));

    double coeff = std::stod(line.substr(24, 11));

    smps.addCoeff(constr, var, coeff);

    // TODO allow second row value?

    return true;
}

bool ColState::maybeTransition(std::unique_ptr<ParserState> &state,
                               std::string &line)
{
    if (line.starts_with("RHS"))
    {
        state = std::make_unique<RhsState>();
        return true;
    }

    return false;
}
