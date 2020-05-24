#ifndef SMIPS_CORE_RHSSTATE_H
#define SMIPS_CORE_RHSSTATE_H

#include "smps/parserstate.h"

namespace smps::core
{
    class RhsState : public smps::ParserState
    {
    public:
        bool parse(smps::Smps &smps, std::string &line) override;

        bool maybeTransition(std::unique_ptr<smps::ParserState> &state,
                             std::string &line) override;
    };
}  // namespace smps::core

#endif  // SMIPS_CORE_RHSSTATE_H
