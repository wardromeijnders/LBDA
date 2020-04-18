#include "decompositions/strongbenders.h"

StrongBenders::~StrongBenders()
{
    delete[] d_z_vars;
    delete[] d_constrs;
}
