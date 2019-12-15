#include "decompositions/pslp.h"

void Pslp::update_tau(double tau_bar)
{
    d_z.set(GRB_DoubleAttr_Obj, tau_bar);
}
