#include "decompositions/zk.h"

bool ZK::is_integer(double val, double precision)
{
    float intpart;
    float frac = std::modf(static_cast<float>(val), &intpart);
    return frac < precision || frac > 1 - precision;
}
