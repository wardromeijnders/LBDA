#include "smps/time/periodstate.h"

#include "smps/time/endatastate.h"

using namespace smps::time;

bool PeriodState::parse(smps::Smps &smps, std::string &line)
{
    std::string const var(line.substr(4, 7));
    std::string const constr(line.substr(14, 7));

    // TODO store period name information? (not really needed, but might be
    //  nice).

    return smps.addStage(constr, var);
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
