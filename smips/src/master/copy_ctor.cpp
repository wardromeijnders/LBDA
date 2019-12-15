#include "master.h"

Master::Master(const Master &other) :
    d_xcoefs(other.d_xcoefs),
    d_cons(other.d_cons),
    d_kappa(other.d_kappa),
    d_beta(other.d_beta),
    d_gamma(other.d_gamma),
    d_n1(other.d_n1),
    d_nSlacks(other.d_nSlacks)
{
    GRBupdatemodel(other.d_cmodel);
    d_cmodel = GRBcopymodel(other.d_cmodel);
}
