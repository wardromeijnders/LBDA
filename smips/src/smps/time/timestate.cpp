#include "smps/time/timestate.h"

#include "smps/time/periodstate.h"

using namespace smps::time;

bool TimeState::parse(smps::Smps &smps, std::string &line)
{
    return smps.name() == smps::trim(line.substr(14, 7));
}

bool TimeState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("PERIODS"))
    {
        state = std::make_unique<PeriodState>();
        return true;
    }

    return false;
}
