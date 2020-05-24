#include "smps/core/namestate.h"
#include "smps/core/rowstate.h"

bool NameState::parse(Smps &smps, std::string &line)
{
    std::string name(line.substr(14, 7));
    smps.setName(name);

    return true;
}

bool NameState::maybeTransition(std::unique_ptr<ParserState> &state,
                                std::string &line)
{
    if (line.starts_with("ROWS"))
    {
        state = std::make_unique<RowState>();
        return true;
    }

    return false;
}
