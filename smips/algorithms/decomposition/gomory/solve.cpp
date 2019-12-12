#include "gomory.h"

double Gomory::solve()
{
    d_model.optimize();

    /*
    double runtime = d_model.get(GRB_DoubleAttr_Runtime);
    if (runtime > 0.1)
      cout << "runtime gomory: " << runtime << '\n';
    */
    /*
    if (d_model.get(GRB_IntAttr_Status) == 9)
      cout << "GOMORY TIMED OUT\n";
    */
    return d_model.get(GRB_DoubleAttr_ObjBound);
}