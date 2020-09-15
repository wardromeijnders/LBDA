#include "smps/timeparser.h"

#include <iterator>

using namespace smps;

void (TimeParser::*(TimeParser::d_actions)[])(smps::DataLine const &)
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

        try
        {
            std::invoke(d_actions[d_state], this, dataLine);
        }
        catch (...)  // bad parse
        {
            throw std::runtime_error("Failed parsing: " + line + '\n');
        }
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

void TimeParser::parseTime(smps::DataLine const &dataLine)
{
    // name field, but this does not really matter as the core file already
    // contains this info.
}

void TimeParser::parsePeriods(smps::DataLine const &dataLine)
{
    d_smps.addStage(dataLine.firstDataName(), dataLine.name());
}
