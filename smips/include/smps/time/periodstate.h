#ifndef SMIPS_TIME_PERIODSTATE_H
#define SMIPS_TIME_PERIODSTATE_H

#include "smps/parserstate.h"

namespace smps::time
{
    class PeriodState : public smps::ParserState
    {
    public:
        bool parse(smps::Smps &smps, std::string &line) override;

        bool maybeTransition(std::unique_ptr<smps::ParserState> &state,
                             std::string &line) override;
    };
}  // namespace smps::time

#endif  // SMIPS_TIME_PERIODSTATE_H
