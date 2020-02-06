#include "decompositions/cglp.h"

double Cglp::solve(double &pi_theta, double *pi_x, double &pi_0)
{
    d_model.optimize();

    pi_theta = d_pi_theta.get(GRB_DoubleAttr_X);
    pi_0 = d_pi_0.get(GRB_DoubleAttr_X);

    double *pi_x_vals = d_model.get(GRB_DoubleAttr_X, d_pi_x, d_n1);
    std::copy(pi_x_vals, pi_x_vals + d_n1, pi_x);
    delete[] pi_x_vals;

    return d_model.get(GRB_DoubleAttr_ObjVal);
}
