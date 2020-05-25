#include "smps/stoch/nonestate.h"

#include "smps/stoch/stochstate.h"

using namespace smps::stoch;

bool NoneState::parse(smps::Smps &smps, std::string &line)
{
    return true;  // NoneState does no parsing.
}

bool NoneState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("STOCH"))
    {
        state = std::make_unique<StochState>();
        return true;
    }

    return false;
}
