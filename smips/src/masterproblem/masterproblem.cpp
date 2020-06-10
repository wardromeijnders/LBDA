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

    for (size_t con = 0; con != Amat.n_cols; ++con)  // constraints
    {
        auto const col = Amat.col(con);
        std::vector<int> ind;
        std::vector<double> val;

        // TODO this is not that nice, I think
        for (auto it = Amat.begin_col(con); it != Amat.end_col(con); ++it)
        {
            ind.push_back(it.row());
            val.push_back(*it);
        }

        GRBaddconstr(d_cmodel,
                     Amat.n_rows,
                     ind.data(),
                     val.data(),
                     GRB_EQUAL,
                     d_problem.d_firstStageRhs(con),
                     nullptr);
    }

    // Add slack variables.
    arma::Col<int> vbeg = arma::linspace<arma::Col<int>>(0,
                                                         d_nSlacks,
                                                         d_nSlacks);

    double vval[d_nSlacks];
    std::fill_n(vTypes, d_problem.d_nFirstStageLeqConstraints, 1.);
    std::fill(vval + d_problem.d_nFirstStageLeqConstraints,
              vval + d_nSlacks,
              -1.);

    GRBaddvars(d_cmodel,
               d_nSlacks,
               d_nSlacks,
               vbeg.memptr(),
               vbeg.memptr(),
               vval,
               nullptr,
               nullptr,
               nullptr,
               nullptr,
               nullptr);

    GRBupdatemodel(d_cmodel);
}
