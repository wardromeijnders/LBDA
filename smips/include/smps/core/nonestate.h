#ifndef SMIPS_CORE_NONESTATE_H
#define SMIPS_CORE_NONESTATE_H

#include "smps/parserstate.h"

namespace smps::core
{
    class NoneState : public smps::ParserState
    {
    public:
        bool parse(smps::Smps &smps, std::string &line) override;

        bool maybeTransition(std::unique_ptr<smps::ParserState> &state,
                             std::string &line) override;
    };
}  // namespace smps::core

#endif  // SMIPS_CORE_NONESTATE_H
