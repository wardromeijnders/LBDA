#include "decompositions/decomposition.h"


double Decomposition::solve()
{
    d_model.optimize();
    return d_model.get(GRB_DoubleAttr_ObjVal);
}
