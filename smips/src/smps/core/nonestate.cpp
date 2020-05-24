#include "smps/core/nonestate.h"

#include "smps/core/namestate.h"


using namespace smps::core;

bool NoneState::parse(smps::Smps &smps, std::string &line)
{
    return true;  // NoneState does no parsing.
}

bool NoneState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("NAME"))
    {
        state = std::make_unique<NameState>();
        return true;
    }

    return false;
}
