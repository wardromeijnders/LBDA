#include "problem.h"

void Problem::setGaussianOmega(double mean, double sd)
{
    d_omega = d_gen.rand_gaus_mat(d_S, d_m2, mean, sd).t();
}
