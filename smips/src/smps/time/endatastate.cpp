#include "smps/time/endatastate.h"

using namespace smps::time;

bool EndataState::parse(smps::Smps &smps, std::string &line)
{
    return true;  // We've already finished parsing.
}

bool EndataState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                  std::string &line)
{
    return false;  // We've already finished parsing.
}
