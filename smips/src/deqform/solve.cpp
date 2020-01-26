#include "deqform.h"

void DeqForm::solve(double time_limit)
{
    d_model.set(GRB_DoubleParam_TimeLimit, time_limit);
    d_model.optimize();

    int status = d_model.get(GRB_IntAttr_Status);

    if (status == GRB_INFEASIBLE)
        d_status = status::INFEASIBLE;
    else
    {
        d_status = status::SOLVED;

        if (d_isMip)
            d_MIPGap = d_model.get(GRB_DoubleAttr_MIPGap);

        d_objVal = d_model.get(GRB_DoubleAttr_ObjVal);
        d_objBound = d_model.get(GRB_DoubleAttr_ObjBound);
        d_xVals = d_model.get(GRB_DoubleAttr_X, d_xVars, d_n1);
        d_runTime = d_model.get(GRB_DoubleAttr_Runtime);
    }
}
