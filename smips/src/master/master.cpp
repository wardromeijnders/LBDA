#include "master.h"

Master::Master(GRBEnv &env, GRBenv *c_env, Problem &problem) :
    d_n1(problem.d_n1),
    d_nSlacks(problem.d_fs_leq + problem.d_fs_geq)
{
    size_t n1 = problem.d_n1;
    size_t m1 = problem.d_m1;
    size_t p1 = problem.d_p1;

    std::vector<std::vector<double>> &Amat = problem.d_Amat;

    double *c = problem.d_c.data();
    double *rhs = problem.d_b.data();

    // instantiating c-api gurobi gurobimodel (in order to use advanced simplex
    // routines)
    GRBnewmodel(c_env, &d_cmodel, NULL, 0, NULL, NULL, NULL, NULL, NULL);
    GRBaddvar(d_cmodel,
              0,
              NULL,
              NULL,
              1.0,
              problem.d_L,
              1e20,
              GRB_CONTINUOUS,
              NULL);  // theta

    char vtypes[n1];
    std::fill_n(vtypes, p1, GRB_INTEGER);
    std::fill(vtypes + p1, vtypes + n1, GRB_CONTINUOUS);

    GRBaddvars(d_cmodel,
               n1,
               0,
               NULL,
               NULL,
               NULL,
               c,
               problem.d_l1.data(),
               problem.d_u1.data(),
               vtypes,
               NULL);  // xvars

    // adding constraints
    int cind[n1];
    std::iota(cind, cind + n1, 1);

    for (size_t con = 0; con != m1; ++con)
        GRBaddconstr(d_cmodel,
                     n1,
                     cind,
                     Amat[con].data(),
                     GRB_EQUAL,
                     rhs[con],
                     NULL);


    // adding slacks
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
               NULL,
               NULL,
               NULL,
               NULL,
               NULL);

    // storing slack identities
    for (size_t con = 0; con != fs_leq; ++con)
    {
        d_kappa.push_back(0);
        d_beta.push_back(Amat[con]);
        d_gamma.push_back(-rhs[con]);
    }

    for (size_t con = fs_leq; con != d_nSlacks; ++con)
    {
        d_kappa.push_back(0);
        std::vector<double> &beta = Amat[con];
        std::vector<double> minus_beta(n1);

        for (size_t var = 0; var != n1; ++var)
            minus_beta[var] = -beta[var];

        d_beta.push_back(minus_beta);
        d_gamma.push_back(rhs[con]);
    }

    GRBupdatemodel(d_cmodel);
}