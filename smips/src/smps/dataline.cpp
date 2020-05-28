#include "smps/dataline.h"

using namespace smps;

DataLine::DataLine(const std::string &line) :
    d_indicator(line.substr(1, 2)),
    d_name(line.substr(4, 7)),
    d_dataName1(line.substr(14, 7)),
    d_numeric1(24, 11)
{
    if (line.length() > 36)
    {
        d_dataName2 = line.substr(39, 7);
        d_numeric2 = line.substr(49, 11);
    }
}
