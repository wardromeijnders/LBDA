#ifndef SMIPS_COREPARSER_H
#define SMIPS_COREPARSER_H

#include "dataline.h"
#include "fileparser.h"

namespace smps
{
    class CoreParser : public FileParser
    {
        using FileParser::FileParser;

        bool d_parseInts = false;  // maybe used when parsing COLUMNS.

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

        static void (CoreParser::*d_actions[])(smps::DataLine const &);

        State d_state = State::NONE;

        void parseName(smps::DataLine const &dataLine);

        void parseRows(smps::DataLine const &dataLine);

        void parseCols(smps::DataLine const &dataLine);

        void parseRhs(smps::DataLine const &dataLine);

        void parseBounds(smps::DataLine const &dataLine);

        void parseRanges(smps::DataLine const &dataLine);

    public:
        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_COREPARSER_H
