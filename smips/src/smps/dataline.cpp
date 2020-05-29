#include "smps/dataline.h"

using namespace smps;

DataLine::DataLine(const std::string &line)
{
    // TODO initialiser lists would be nice, but that's not the world we live
    //  in, unfortunately.
    d_indicator = line.substr(1, 2);
    d_name = line.substr(4, 7);

    if (line.length() >= 14)
        d_dataName1 = line.substr(14, 7);

    if (line.length() >= 24)
        d_numeric1 = line.substr(24, 11);

    if (line.length() >= 39)
        d_dataName2 = line.substr(39, 7);

    if (line.length() >= 49)
        d_numeric2 = line.substr(49, 11);
}

std::string const &DataLine::indicator() const
{
    return d_indicator;
}

std::string const &DataLine::name() const
{
    return d_name;
}

std::string const &DataLine::firstDataName() const
{
    return d_dataName1;
}

std::pair<std::string, double> DataLine::firstDataEntry() const
{
    return std::make_pair(d_dataName1, std::stod(d_numeric1));
}

bool DataLine::hasSecondDataEntry() const
{
    return !d_dataName2.empty();
}

std::string const &DataLine::secondDataName() const
{
    return d_dataName2;
}

std::pair<std::string, double> DataLine::secondDataEntry() const
{
    return std::make_pair(d_dataName2, std::stod(d_numeric2));
}
