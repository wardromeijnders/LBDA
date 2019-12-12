#include "cglp.h"

Cglp::~Cglp()
{
  delete[] d_pi_x;
  for (size_t term = 0; term !=  2 * d_n1; ++term)
    delete[] d_constrs_x[term];
}