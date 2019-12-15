#include "deqform.h"

DeqForm::~DeqForm()
{
    if (d_status == status::SOLVED)
        delete[] d_xVals;

    delete[] d_xVars;
}
