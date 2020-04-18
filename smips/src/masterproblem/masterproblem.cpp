#include "masterproblem.h"

MasterProblem::MasterProblem(GRBenv *c_env, Problem &problem) :
    d_problem(problem),
    d_nSlacks(problem.d_nFirstStageLeqConstraints
              + problem.d_nFirstStageGeqConstraints)
{
    auto &Amat = d_problem.Amat();

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
              d_problem.d_L,
              arma::datum::inf,
              GRB_CONTINUOUS,
              nullptr);

    char vTypes[Amat.n_rows];
    std::fill_n(vTypes, d_problem.nFirstStageIntVars(), GRB_INTEGER);
    std::fill(vTypes + d_problem.nFirstStageIntVars(),
              vTypes + Amat.n_rows,
              GRB_CONTINUOUS);

    GRBaddvars(d_cmodel,  // first-stage (x) variables.
               Amat.n_rows,
               0,
               nullptr,
               nullptr,
               nullptr,
               d_problem.d_firstStageCoeffs.memptr(),
               d_problem.d_firstStageLowerBound.memptr(),
               d_problem.d_firstStageUpperBound.memptr(),
               vTypes,
               nullptr);

    arma::Col<int> cind = arma::ones<arma::Col<int>>(Amat.n_rows);

    for (size_t con = 0; con != Amat.n_cols; ++con)  // constraints
        GRBaddconstr(d_cmodel,
                     Amat.n_rows,
                     cind.memptr(),
                     Amat.colptr(con),
                     GRB_EQUAL,
                     d_problem.d_firstStageRhs(con),
                     nullptr);

    // Add slack variables.
    arma::Col<int> vbeg = arma::linspace<arma::Col<int>>(0,
                                                         d_nSlacks,
                                                         d_nSlacks);

    arma::vec vval(d_nSlacks);
    vval.head(d_problem.d_nFirstStageLeqConstraints).fill(1);
    vval.tail(d_nSlacks - d_problem.d_nFirstStageLeqConstraints).fill(-1);

    GRBaddvars(d_cmodel,
               d_nSlacks,
               d_nSlacks,
               vbeg.memptr(),
               vbeg.memptr(),
               vval.memptr(),
               nullptr,
               nullptr,
               nullptr,
               nullptr,
               nullptr);

    GRBupdatemodel(d_cmodel);
}
