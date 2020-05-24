#include "smps/core/endatastate.h"


bool EndataState::parse(Smps &smps, std::string &line)
{
    return true;  // We've already finished parsing.
}

bool EndataState::maybeTransition(std::unique_ptr<ParserState> &state,
                                  std::string &line)
{
    return false;  // We've already finished parsing.
}
