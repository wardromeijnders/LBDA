#ifndef SMIPS_COREPARSER_H
#define SMIPS_COREPARSER_H

#include "dataline.h"
#include "smps.h"

namespace smps
{
    class CoreParser
    {
        enum State
        {
            NONE,
            NAME,
            ROWS,
            COLS,
            RHS,
            BOUNDS,
            RANGES,
            ENDATA
        };

        static bool (CoreParser::*d_actions[])(smps::DataLine const &);

        smps::Smps &d_smps;

        State d_state = State::NONE;

        bool parseNone(smps::DataLine const &dataLine);

        bool parseName(smps::DataLine const &dataLine);

        bool parseRows(smps::DataLine const &dataLine);

        bool parseCols(smps::DataLine const &dataLine);

        bool parseRhs(smps::DataLine const &dataLine);

        bool parseBounds(smps::DataLine const &dataLine);

        bool parseRanges(smps::DataLine const &dataLine);

        bool parseEndata(smps::DataLine const &dataLine);

    public:
        CoreParser(Smps &smps);

        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_COREPARSER_H
