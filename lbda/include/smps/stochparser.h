#ifndef SMIPS_STOCHPARSER_H
#define SMIPS_STOCHPARSER_H

#include "dataline.h"
#include "fileparser.h"

namespace smps
{
    class StochParser : public FileParser
    {
        using FileParser::FileParser;

        std::string d_scenarioName;  // used when parsing SCENARIOS.

        enum State
        {
            NONE,
            STOCH,
            INDEP,
            BLOCKS,
            SCENARIOS,
            ENDATA
        };

        static void (StochParser::*d_actions[])(smps::DataLine const &);

        State d_state = State::NONE;

        void parseStoch(smps::DataLine const &dataLine);

        void parseIndep(smps::DataLine const &dataLine);

        void parseBlocks(smps::DataLine const &dataLine);

        void parseScenarios(smps::DataLine const &dataLine);

    public:
        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_STOCHPARSER_H
