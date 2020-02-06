#include "decompositions/relaxation.h"


double Relaxation::solve()
{
    d_model.optimize();
    return d_model.get(GRB_DoubleAttr_ObjVal);
}
