#ifndef SMIPS_FILEPARSER_H
#define SMIPS_FILEPARSER_H

#include "dataline.h"
#include "smps.h"

namespace smps
{
    class FileParser
    {
    protected:
        Smps &d_smps;  // reference to the SMPS object to be populated

        bool parseNone(smps::DataLine const &dataLine);

        bool parseEndata(smps::DataLine const &dataLine);

    public:
        explicit FileParser(Smps &smps);
    };
}  // namespace smps

#endif  // SMIPS_FILEPARSER_H
