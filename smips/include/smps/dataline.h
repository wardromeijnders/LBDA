#ifndef SMIPS_DATALINE_H
#define SMIPS_DATALINE_H

#include <string>

namespace smps
{
    class DataLine
    {
        std::string d_indicator;
        std::string d_name;

        std::string d_dataName1;
        std::string d_numeric1;

        std::string d_dataName2;
        std::string d_numeric2;

    public:
        DataLine(std::string const &line);

        std::string const &indicator() const;

        std::string const &name() const;

        std::string const &firstDataName() const;

        std::pair<std::string, double> firstDataEntry() const;

        bool hasSecondDataEntry() const;

        std::string const &secondDataName() const;

        std::pair<std::string, double> secondDataEntry() const;
    };
}  // namespace smps

#endif  // SMIPS_DATALINE_H
