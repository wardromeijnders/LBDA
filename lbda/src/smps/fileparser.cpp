#include "smps/fileparser.h"

using namespace smps;

FileParser::FileParser(Smps &smps) : d_smps(smps)
{
}

void FileParser::parseNone(smps::DataLine const &dataLine)
{
    // nothing to do in initial state.
}

void FileParser::parseEndata(smps::DataLine const &dataLine)
{
    // finished parsing.
}
