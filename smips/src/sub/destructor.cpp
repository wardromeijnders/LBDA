#include "sub.h"

Sub::~Sub()
{
    delete[] d_vars;
    delete[] d_constrs;
}
