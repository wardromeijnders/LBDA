#include "decompositions/ald.h"

Ald::~Ald()
{
    GRBfreemodel(d_model);
}
