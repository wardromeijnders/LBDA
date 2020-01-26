#include "decompositions/cglp.h"

void Cglp::changeLastRow(double *x_coefs, double cons)
{
    for (size_t term = 0; term != 2 * d_n1; ++term)
    {
        d_model.chgCoeff(d_constrs_cons[term], d_lambda, cons);

        GRBVar lambda_vector[d_n1];
        std::fill_n(lambda_vector, d_n1, d_lambda);

        d_model.chgCoeffs(d_constrs_x[term], lambda_vector, x_coefs, d_n1);
    }
}
