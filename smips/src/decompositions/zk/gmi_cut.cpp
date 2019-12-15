#include "decompositions/zk.h"

void ZK::gmi_cut(double *tab_row_x,
                 double *tab_row_y,
                 double a0,
                 double *coef_x,
                 double *coef_y,
                 double &coef_theta,
                 int nVarsMaster)
{
    double f0 = a0 - floor(a0);

    // theta
    double a_theta = tab_row_x[0];
    coef_theta = std::max(a_theta / f0, -a_theta / (1 - f0));

    // x variables
    for (size_t var = 1; var != nVarsMaster; ++var)
    {
        size_t xvar = var - 1;

        double aj = tab_row_x[var];
        if (xvar < d_p1)  // check if integer variable
        {
            double fj = aj - floor(aj);
            coef_x[xvar] = std::min(fj / f0, (1 - fj) / (1 - f0));
        }
        else
            coef_x[xvar] = std::max(aj / f0, -aj / (1 - f0));
    }

    // y variables
    for (size_t var = 0; var != d_nVars; ++var)
    {
        double aj = tab_row_y[var];

        if (var < d_p2)
        {
            double fj = aj - floor(aj);
            coef_y[var] = std::min(fj / f0, (1 - fj) / (1 - f0));
        }
        else
            coef_y[var] = std::max(aj / f0, -aj / (1 - f0));
    }
}