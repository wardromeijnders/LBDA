#include "smps/time/periodstate.h"

#include "smps/time/endatastate.h"

using namespace smps::time;

bool PeriodState::parse(smps::Smps &smps, std::string &line)
{
    // TODO
    return false;
}

bool PeriodState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                  std::string &line)
{
    if (line.starts_with("ENDATA"))
    {
        state = std::make_unique<EndataState>();
        return true;
    }

    return false;
}
