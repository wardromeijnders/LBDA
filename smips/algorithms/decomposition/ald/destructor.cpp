#include "ald.h"

Ald::~Ald()
{
  //cout << "ald destructor called\n";
  GRBfreemodel(d_model);
}