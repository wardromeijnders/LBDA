#ifndef SMIPS_CORE_ROWSTATE_H
#define SMIPS_CORE_ROWSTATE_H

#include "smps/parserstate.h"


class RowState : public ParserState
{
public:
    bool parse(Smps &smps, std::string &line) override;

    bool maybeTransition(std::unique_ptr<ParserState> &state,
                         std::string &line) override;
};


#endif  // SMIPS_CORE_ROWSTATE_H
