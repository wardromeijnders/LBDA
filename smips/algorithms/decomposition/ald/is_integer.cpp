#include "ald.h"

bool Ald::is_integer(double val)
{
  float intpart;
  float frac = modf(val, &intpart);
  return frac == 0 || frac == 1;
}