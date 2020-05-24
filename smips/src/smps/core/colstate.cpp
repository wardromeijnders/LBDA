#include "smps/core/colstate.h"

#include "smps/core/rhsstate.h"

using namespace smps::core;

bool ColState::parse(smps::Smps &smps, std::string &line)
{
    std::string var(line.substr(4, 7));
    std::string constr(line.substr(14, 7));

    double coeff = std::stod(line.substr(24, 11));

    smps.addCoeff(constr, var, coeff);

    if (line.size() <= 36)
        return true;

    constr = line.substr(39, 7);
    coeff = std::stod(line.substr(49, 11));

    smps.addCoeff(constr, var, coeff);

    return true;
}

bool ColState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                               std::string &line)
{
    if (line.starts_with("RHS"))
    {
        state = std::make_unique<core::RhsState>();
        return true;
    }

    return false;
}
