#ifndef SMIPS_COREPARSER_H
#define SMIPS_COREPARSER_H

#include "dataline.h"
#include "smps.h"

namespace smps
{
    class CoreParser
    {
        smps::Smps *d_smps;

        bool parseDataLine(smps::DataLine const &dataLine);

    public:
        void parse(std::string const &location);
    };
}  // namespace smps

#endif  // SMIPS_COREPARSER_H
