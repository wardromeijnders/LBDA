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

void StochParser::parse(std::string const &location)
{
    std::ifstream file(location);

    if (file.fail())
        throw std::runtime_error("Could not open .sto file.");

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

bool StochParser::transition(std::string const &line)
{
    std::string names[]
        = {"NONE", "STOCH", "INDEP", "BLOCKS", "SCENARIOS", "ENDATA"};

    for (size_t idx = 0; idx != std::size(names); ++idx)
        if (line.rfind(names[idx], 0) == 0)
        {
            if (idx > 1 && idx < 5 && line.find("DISCRETE") == std::string::npos)
                std::cerr << "Stoch file: DISCRETE was not specified, but is"
                             " assumed.\n";

            d_state = static_cast<State>(idx);
            return true;
        }

    return false;
}

bool StochParser::parseStoch(const smps::DataLine &dataLine)
{
    return d_smps.name() == dataLine.firstDataName();
}

bool StochParser::parseIndep(smps::DataLine const &dataLine)
{
    if (dataLine.name() != "RHS")
    {
        std::cerr << "SMIPS currently understands only stochastic RHS.\n";
        return false;
    }

    auto [constr, value] = dataLine.firstDataEntry();
    auto [_, prob] = dataLine.secondDataEntry();

    return d_smps.addIndep(constr, std::make_pair(value, prob));
}

bool StochParser::parseBlocks(smps::DataLine const &dataLine)
{
    if (dataLine.name() != "RHS")
    {
        std::cerr << "SMIPS currently understands only stochastic RHS.\n";
        return false;
    }

    return false;  // TODO
}

bool StochParser::parseScenarios(smps::DataLine const &dataLine)
{
    if (dataLine.indicator() == "SC")  // new scenario
    {
        d_scenarioName = dataLine.name();
        auto const &[_, prob] = dataLine.firstDataEntry();

        return d_smps.addScenario(d_scenarioName, prob);
    }
    else if (dataLine.name() != "RHS")
    {
        std::cerr << "SMIPS currently understands only stochastic RHS.\n";
        return false;
    }

    auto const &[constr, value] = dataLine.firstDataEntry();
    return d_smps.addScenarioRealisation(d_scenarioName, constr, value);
}
