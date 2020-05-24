#include "smps/time/nonestate.h"

#include "smps/time/timestate.h"

using namespace smps::time;

bool NoneState::parse(smps::Smps &smps, std::string &line)
{
    return true;  // NoneState does no parsing.
}

bool NoneState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("TIME"))
    {
        state = std::make_unique<TimeState>();
        return true;
    }

    return false;
}
