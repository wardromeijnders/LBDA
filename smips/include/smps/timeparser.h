#ifndef SMIPS_TIMEPARSER_H
#define SMIPS_TIMEPARSER_H

#include "dataline.h"
#include "smps.h"

namespace smps
{
    class TimeParser
    {
        enum State
        {
            NONE,
            TIME,
            PERIODS,
            ENDATA
        };

        static bool (TimeParser::*d_actions[])(smps::DataLine const &);

        smps::Smps &d_smps;

        State d_state = State::NONE;

        bool parseNone(smps::DataLine const &dataLine);

        bool parseTime(smps::DataLine const &dataLine);

        bool parsePeriods(smps::DataLine const &dataLine);

        bool parseEndata(smps::DataLine const &dataLine);

    public:
        TimeParser(Smps &smps);

        void parse(std::string const &location);

        bool transition(std::string const &line);
    };
}  // namespace smps

#endif  // SMIPS_TIMEPARSER_H
