#include "smps/fileparser.h"

using namespace smps;

FileParser::FileParser(Smps &smps) : d_smps(smps)
{
}

bool FileParser::parseNone(smps::DataLine const &dataLine)
{
    return true;  // nothing to do in initial state.
}

bool FileParser::parseEndata(smps::DataLine const &dataLine)
{
    return true;  // finished parsing.
}
