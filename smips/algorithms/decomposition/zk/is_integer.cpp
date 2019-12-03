#include "zk.h"

bool ZK::is_integer(double val, double precision)
{
  float intpart;
  float frac = modf(val, &intpart);
  return frac < precision || frac > 1 - precision;
  //return frac == 0 || frac == 1;
}