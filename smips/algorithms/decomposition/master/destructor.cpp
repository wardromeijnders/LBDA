#include "master.h"

Master::~Master()
{
  //cout << "master destructor called\n";
  GRBfreemodel(d_cmodel);
}