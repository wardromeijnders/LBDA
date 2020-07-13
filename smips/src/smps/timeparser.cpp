#include "smps/timeparser.h"

#include <iterator>

using namespace smps;

bool (TimeParser::*(TimeParser::d_actions)[])(smps::DataLine const &)
    = {&TimeParser::parseNone,
       &TimeParser::parseTime,
       &TimeParser::parsePeriods,
       &TimeParser::parseEndata};

void TimeParser::parse(std::string const &location)
{
    std::ifstream file(location);

    if (file.fail())
        throw std::runtime_error("Could not open .tim file.");

    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind('*', 0) == 0)  // comment
            continue;

        if (transition(line))  // header row
            continue;

        DataLine const dataLine(line);

        if (!std::invoke(d_actions[d_state], this, dataLine))  // bad parse
            std::cerr << "Failed parsing: " << line << '\n';
    }
}

bool TimeParser::transition(std::string const &line)
{
    std::string names[] = {"NONE", "TIME", "PERIODS", "ENDATA"};

    for (size_t idx = 0; idx != std::size(names); ++idx)
        if (line.rfind(names[idx], 0) == 0)
        {
            d_state = static_cast<State>(idx);
            return true;
        }

    return false;
}

bool TimeParser::parseTime(smps::DataLine const &dataLine)
{
    return d_smps.name() == dataLine.firstDataName();
}

bool TimeParser::parsePeriods(smps::DataLine const &dataLine)
{
    return d_smps.addStage(dataLine.firstDataName(), dataLine.name());
}
