#include "lagrangian.h"

Lagrangian::~Lagrangian()
{
  //cout << "lagrangian destructor called\n";
  delete[] d_z_vars; 
  delete[] d_constrs; 
}