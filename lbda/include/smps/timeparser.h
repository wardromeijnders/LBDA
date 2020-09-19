#ifndef SMIPS_TIMEPARSER_H
#define SMIPS_TIMEPARSER_H

#include "dataline.h"
#include "fileparser.h"

namespace smps
{
    class TimeParser : public FileParser
    {
        using FileParser::FileParser;

        enum State
        {
            NONE,
            TIME,
            PERIODS,
            ENDATA
        };

        static void (TimeParser::*d_actions[])(smps::DataLine const &);

        State d_state = State::NONE;

        void parseTime(smps::DataLine const &dataLine);

        void parsePeriods(smps::DataLine const &dataLine);

    public:
        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_TIMEPARSER_H
