#include "problem.h"

Problem::~Problem()
{
  if (d_sub_initialized)
    delete[] d_constrs;   
}
