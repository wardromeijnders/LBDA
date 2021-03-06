#include "smps/dataline.h"

using namespace smps;

DataLine::DataLine(const std::string &line)
{
    d_indicator = trim(line.substr(1, 2));
    d_name = trim(line.substr(4, 8));

    if (line.length() >= 15)
        d_dataName1 = trim(line.substr(14, 8));

    if (line.length() >= 25)
        d_numeric1 = trim(line.substr(24, 12));

    if (line.length() >= 40)
        d_dataName2 = trim(line.substr(39, 8));

    if (line.length() >= 50)
        d_numeric2 = trim(line.substr(49, 12));
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
    return !d_dataName2.empty() && !d_numeric2.empty();
}

std::string const &DataLine::secondDataName() const
{
    return d_dataName2;
}

std::pair<std::string, double> DataLine::secondDataEntry() const
{
    return std::make_pair(d_dataName2, std::stod(d_numeric2));
}

std::string DataLine::trim(std::string const &target)
{
    size_t first = target.find_first_not_of(' ');

    if (first == std::string::npos)
        return target;

    size_t last = target.find_last_not_of(' ');

    return target.substr(first, last - first + 1);
}
