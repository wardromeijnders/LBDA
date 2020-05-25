#include "smps/stoch/stochstate.h"


using namespace smps::stoch;

bool StochState::parse(smps::Smps &smps, std::string &line)
{
    return smps.name() == smps::trim(line.substr(14, 7));
}

bool StochState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                 std::string &line)
{
    // TODO INDEP/SCENARIOS

    return false;
}
