#ifndef SMIPS_STOCHPARSER_H
#define SMIPS_STOCHPARSER_H

#include "dataline.h"
#include "fileparser.h"

namespace smps
{
    class StochParser : public FileParser
    {
        using FileParser::FileParser;

        enum State
        {
            NONE,
            STOCH,
            INDEP,
            BLOCKS,
            SCENARIOS,
            ENDATA
        };

        static bool (StochParser::*d_actions[])(smps::DataLine const &);

        State d_state = State::NONE;

        bool parseStoch(smps::DataLine const &dataLine);

        bool parseIndep(smps::DataLine const &dataLine);

        bool parseBlocks(smps::DataLine const &dataLine);

        bool parseScenarios(smps::DataLine const &dataLine);

    public:
        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_STOCHPARSER_H
