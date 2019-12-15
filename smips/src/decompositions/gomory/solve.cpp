#include "decompositions/gomory.h"

double Gomory::solve()
{
    d_model.optimize();
    return d_model.get(GRB_DoubleAttr_ObjBound);
}
