#include "zk.h"

ZK::~ZK()
{
  if (d_model)
    GRBfreemodel(d_model);
}