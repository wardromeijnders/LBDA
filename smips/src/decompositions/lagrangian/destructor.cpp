#include "decompositions/lagrangian.h"

Lagrangian::~Lagrangian()
{
    delete[] d_z_vars;
    delete[] d_constrs;
}
