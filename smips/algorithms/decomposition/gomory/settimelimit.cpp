#include "gomory.h"

void Gomory::setTimeLimit(double timeLimit)
{
  d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
}