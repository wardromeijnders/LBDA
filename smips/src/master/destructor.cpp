#include "master.h"

Master::~Master()
{
    GRBfreemodel(d_cmodel);
}
