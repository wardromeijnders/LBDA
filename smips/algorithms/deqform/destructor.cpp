#include "deqform.h"

DeqForm::~DeqForm()
{
  if (d_status == 0)
    delete[] d_xVals;
    
  delete[] d_xVars;     
}
