#include "smps/core/nonestate.h"

#include "smps/core/namestate.h"


bool NoneState::parse(Smps &smps, std::string &line)
{
    return true;  // NoneState does no parsing.
}

bool NoneState::maybeTransition(std::unique_ptr<ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("NAME"))
    {
        state = std::make_unique<NameState>();
        return true;
    }

    return false;
}
