#include "smps/coreparser.h"

using namespace smps;

void CoreParser::parse(std::string const &location)
{
    std::ifstream file(location);
    std::string line;

    // TODO
    while (std::getline(file, line))
    {
        if (line.starts_with('*'))  // comment
            continue;

        if (state->maybeTransition(state, line))  // header row
            continue;

        if (!state->parse(*this, line))  // could not parse - why?
            std::cerr << "Failed parsing: " << line << '\n';
    }
}

bool CoreParser::parseDataLine(DataLine const &dataLine)
{
    // TODO
}
