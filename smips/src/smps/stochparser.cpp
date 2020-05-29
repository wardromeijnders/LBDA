#include "smps/stochparser.h"

#include <iterator>

using namespace smps;

bool (StochParser::*(StochParser::d_actions)[])(smps::DataLine const &)
    = {&StochParser::parseNone,
       &StochParser::parseStoch,
       &StochParser::parseIndep,
       &StochParser::parseBlocks,
       &StochParser::parseScenarios,
       &StochParser::parseEndata};

StochParser::StochParser(Smps &smps) : d_smps(smps)
{
}

void StochParser::parse(std::string const &location)
{
    std::ifstream file(location);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.starts_with('*'))  // comment
            continue;

        if (transition(line))  // header row
            continue;

        DataLine const dataLine(line);

        if (!std::invoke(d_actions[d_state], this, dataLine))  // bad parse
            std::cerr << "Failed parsing: " << line << '\n';
    }
}

bool StochParser::transition(std::string const &line)
{
    std::string names[]
        = {"NONE", "STOCH", "INDEP", "BLOCKS", "SCENARIOS", "ENDATA"};

    for (size_t idx = 0; idx != std::size(names); ++idx)
        if (line.starts_with(names[idx]))
        {
            d_state = static_cast<State>(idx);
            return true;
        }

    return false;
}

bool StochParser::parseNone(smps::DataLine const &dataLine)
{
    return true;  // nothing to do in initial state.
}

bool StochParser::parseStoch(const smps::DataLine &dataLine)
{
    return d_smps.addStage(dataLine.firstDataName(), dataLine.name());
}

bool StochParser::parseIndep(smps::DataLine const &dataLine)
{
    return false;  // TODO
}

bool StochParser::parseBlocks(smps::DataLine const &dataLine)
{
    return false;  // TODO
}

bool StochParser::parseScenarios(smps::DataLine const &dataLine)
{
    return false;  // TODO
}

bool StochParser::parseEndata(smps::DataLine const &dataLine)
{
    return true;  // finished parsing.
}
