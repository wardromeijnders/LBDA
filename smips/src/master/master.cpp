#include "master.h"

Master::Master(GRBEnv &env, GRBenv *c_env, Problem &problem) :
    d_n1(problem.d_n1),
    d_nSlacks(problem.d_fs_leq + problem.d_fs_geq)
{
    GRBnewmodel(c_env,  // The C API gives access to advanced simplex routines.
                &d_cmodel,
                nullptr,
                0,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr);

    GRBaddvar(d_cmodel,  // theta
              0,
              nullptr,
              nullptr,
              1.0,
              problem.d_L,
              1e20,
              GRB_CONTINUOUS,
              nullptr);

    char vtypes[d_n1];
    std::fill_n(vtypes, problem.d_p1, GRB_INTEGER);
    std::fill(vtypes + problem.d_p1, vtypes + d_n1, GRB_CONTINUOUS);

    GRBaddvars(d_cmodel,  // first-stage (x) variables.
               d_n1,
               0,
               nullptr,
               nullptr,
               nullptr,
               problem.d_c.data(),
               problem.d_l1.data(),
               problem.d_u1.data(),
               vtypes,
               nullptr);

    // Add constraints.
    int cind[d_n1];
    std::iota(cind, cind + d_n1, 1);

    double *rhs = problem.d_b.data();

    for (size_t con = 0; con != problem.d_m1; ++con)
        GRBaddconstr(d_cmodel,
                     d_n1,
                     cind,
                     problem.d_Amat[con].data(),
                     GRB_EQUAL,
                     rhs[con],
                     nullptr);

    // Add slack variables.
    int vbeg[d_nSlacks];
    std::iota(vbeg, vbeg + d_nSlacks, 0);

    size_t fs_leq = problem.d_fs_leq;
    size_t fs_geq = problem.d_fs_geq;

    int *vind = vbeg;
    double vval[d_nSlacks];
    std::fill_n(vval, fs_leq, 1);
    std::fill_n(vval + fs_leq, fs_geq, -1);

    GRBaddvars(d_cmodel,
               d_nSlacks,
               d_nSlacks,
               vbeg,
               vind,
               vval,
               nullptr,
               nullptr,
               nullptr,
               nullptr,
               nullptr);

    // Store slack identities
    for (size_t con = 0; con != fs_leq; ++con)
    {
        d_kappa.push_back(0);
        d_beta.push_back(problem.d_Amat[con]);
        d_gamma.push_back(-rhs[con]);
    }

    for (size_t con = fs_leq; con != d_nSlacks; ++con)
    {
        d_kappa.push_back(0);
        std::vector<double> &beta = problem.d_Amat[con];
        std::vector<double> minus_beta(d_n1);

        for (size_t var = 0; var != d_n1; ++var)
            minus_beta[var] = -beta[var];

        d_beta.push_back(minus_beta);
        d_gamma.push_back(rhs[con]);
    }

    GRBupdatemodel(d_cmodel);
}
