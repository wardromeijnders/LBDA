#ifndef SMIPS_STOCHPARSER_H
#define SMIPS_STOCHPARSER_H

#include "dataline.h"
#include "smps.h"

namespace smps
{
    class StochParser
    {
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

        smps::Smps &d_smps;

        State d_state = State::NONE;

        bool parseNone(smps::DataLine const &dataLine);

        bool parseStoch(smps::DataLine const &dataLine);

        bool parseIndep(smps::DataLine const &dataLine);

        bool parseBlocks(smps::DataLine const &dataLine);

        bool parseScenarios(smps::DataLine const &dataLine);

        bool parseEndata(smps::DataLine const &dataLine);

    public:
        StochParser(Smps &smps);

        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_STOCHPARSER_H
