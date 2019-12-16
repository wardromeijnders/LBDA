#include "decompositions/zk.h"

ZK::ZK(GRBenv *env, Problem &problem, size_t scenario) :
    d_n1(problem.d_n1),
    d_p1(problem.d_p1),
    d_p2(problem.d_p2),
    d_nVars(problem.d_n2 + problem.d_ss_leq + problem.d_ss_geq),
    d_nConstrs(problem.d_m2),
    d_omega(problem.d_omega[scenario]),
    d_Tmat(problem.d_Tmat),
    d_tau(problem.d_m2)
{
    GRBnewmodel(env, &d_model, NULL, 0, NULL, NULL, NULL, NULL, NULL);

    // adding variables  (assumed continuous, lower and upper bounds ignored)
    GRBaddvars(d_model,
               problem.d_n2,
               0,
               NULL,
               NULL,
               NULL,
               problem.d_q.data(),
               NULL,
               NULL,
               NULL,
               NULL);

    // adding constraints
    int cind[problem.d_n2];
    std::iota(cind, cind + problem.d_n2, 0);

    for (size_t con = 0; con != problem.d_m2; ++con)
        GRBaddconstr(d_model,
                     problem.d_n2,
                     cind,
                     problem.d_Wmat[con].data(),
                     GRB_EQUAL,
                     0,
                     NULL);

    int nSlacks = problem.d_ss_leq + problem.d_ss_geq;

    // number of second-stage variables (including slacks)
    d_n2 = problem.d_n2 + nSlacks;

    // adding slacks
    int vbeg[nSlacks];
    std::iota(vbeg, vbeg + nSlacks, 0);

    double vval[nSlacks];
    std::fill_n(vval, problem.d_ss_leq, 1);
    std::fill_n(vval + problem.d_ss_leq, problem.d_ss_geq, -1);

    GRBaddvars(d_model,
               nSlacks,
               nSlacks,
               vbeg,
               vbeg,
               vval,
               NULL,
               NULL,
               NULL,
               NULL,
               NULL);

    GRBupdatemodel(d_model);
}