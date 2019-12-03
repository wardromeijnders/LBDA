#include "sub.h"

Sub::~Sub()
{
  //cout << "sub destructor called\n";
  delete[] d_vars; 
  delete[] d_constrs; 
}