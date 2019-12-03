#include "gomory.h"

Gomory::~Gomory()
{  
  //cout << "gomory destructor called\n";
  delete[] d_vars; 
  delete[] d_constrs; 
}