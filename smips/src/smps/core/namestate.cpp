#include "smps/core/namestate.h"
#include "smps/core/rowstate.h"


using namespace smps::core;

bool NameState::parse(smps::Smps &smps, std::string &line)
{
    std::string name(line.substr(14, 7));
    smps.setName(name);

    return true;
}

bool NameState::maybeTransition(std::unique_ptr<smps::ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("ROWS"))
    {
        state = std::make_unique<RowState>();
        return true;
    }

    return false;
}
