#include "benders.h"

void Benders::computeTx(double const *x, double *Tx)
{
    std::vector<std::vector<double>> &Tmat = d_problem.d_Tmat;

    for (size_t zvar = 0; zvar != d_m2; ++zvar)
    {
        Tx[zvar] = 0.0;
        for (size_t xvar = 0; xvar != d_n1; ++xvar)
            Tx[zvar] += Tmat[zvar][xvar] * x[xvar];
    }
}