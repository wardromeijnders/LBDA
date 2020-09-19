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

        void parseNone(smps::DataLine const &dataLine);

        void parseEndata(smps::DataLine const &dataLine);

    public:
        explicit FileParser(Smps &smps);
    };
}  // namespace smps

#endif  // SMIPS_FILEPARSER_H
