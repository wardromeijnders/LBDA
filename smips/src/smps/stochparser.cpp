#include "smps/stochparser.h"

#include <iterator>

using namespace smps;

void (StochParser::*(StochParser::d_actions)[])(smps::DataLine const &)
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

void StochParser::parseStoch(const smps::DataLine &dataLine)
{
    // name field, but this does not really matter as the core file already
    // contains this info.
}

void StochParser::parseIndep(smps::DataLine const &dataLine)
{
    if (dataLine.name() != "RHS")
    {
        std::cerr << "SMIPS currently understands only stochastic RHS.\n";
        return;
    }

    auto [constr, value] = dataLine.firstDataEntry();
    auto [_, prob] = dataLine.secondDataEntry();

    d_smps.addIndep(constr, std::make_pair(value, prob));
}

void StochParser::parseBlocks(smps::DataLine const &dataLine)
{
    if (dataLine.name() != "RHS")
    {
        std::cerr << "SMIPS currently understands only stochastic RHS.\n";
        return;
    }
}

void StochParser::parseScenarios(smps::DataLine const &dataLine)
{
    if (dataLine.indicator() == "SC")  // new scenario
    {
        d_scenarioName = dataLine.name();
        auto const &[_, prob] = dataLine.firstDataEntry();

        d_smps.addScenario(d_scenarioName, prob);
    }
    else if (dataLine.name() != "RHS")
    {
        std::cerr << "SMIPS currently understands only stochastic RHS.\n";
    }

    auto const &[constr, value] = dataLine.firstDataEntry();
    d_smps.addScenarioRealisation(d_scenarioName, constr, value);
}
