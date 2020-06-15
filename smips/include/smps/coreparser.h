#ifndef SMIPS_COREPARSER_H
#define SMIPS_COREPARSER_H

#include "dataline.h"
#include "fileparser.h"

namespace smps
{
    class CoreParser : public FileParser
    {
        using FileParser::FileParser;

        bool d_parseInts = false;

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

        State d_state = State::NONE;

        bool parseName(smps::DataLine const &dataLine);

        bool parseRows(smps::DataLine const &dataLine);

        bool parseCols(smps::DataLine const &dataLine);

        bool parseRhs(smps::DataLine const &dataLine);

        bool parseBounds(smps::DataLine const &dataLine);

        bool parseRanges(smps::DataLine const &dataLine);

    public:
        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_COREPARSER_H
