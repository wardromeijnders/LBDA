#ifndef SMIPS_PARSERSTATE_H
#define SMIPS_PARSERSTATE_H

#include "smps.h"

#include <iosfwd>
#include <memory>

namespace smps
{
    class ParserState
    {
    public:
        virtual bool parse(Smps &smps, std::string &line) = 0;

        virtual bool maybeTransition(std::unique_ptr<ParserState> &state,
                                     std::string &line)
            = 0;

        virtual ~ParserState() = default;
    };
}  // namespace smps

#endif  // SMIPS_PARSERSTATE_H
