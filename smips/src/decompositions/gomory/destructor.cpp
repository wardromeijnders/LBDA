#include "decompositions/gomory.h"

Gomory::~Gomory()
{
    delete[] d_vars;
    delete[] d_constrs;
}
