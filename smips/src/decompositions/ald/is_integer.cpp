#include "decompositions/ald.h"

bool Ald::is_integer(double val)
{
    float intpart;
    float frac = std::modf(static_cast<float>(val), &intpart);
    return frac == 0 || frac == 1;
}
