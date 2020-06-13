#include "masterproblem.h"

MasterProblem::MasterProblem(GRBenv *c_env, Problem &problem) :
    d_problem(problem)
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
            ind.emplace_back(it.row());
            val.emplace_back(*it);
        }

        GRBaddconstr(d_cmodel,
                     Amat.n_rows,
                     ind.data(),
                     val.data(),
                     GRB_EQUAL,
                     d_problem.d_firstStageRhs(con),
                     nullptr);
    }

    std::vector<int> vbeg;
    std::vector<double> vval;

    auto const &senses = d_problem.firstStageConstrSenses();

    for (size_t idx = 0; idx != senses.size(); ++idx)
        if (senses[idx] != GRB_EQUAL)
        {
            d_nSlacks++;

            // Is either <= or >=.
            vval.emplace_back(senses[idx] == GRB_LESS_EQUAL ? 1. : -1.);
            vbeg.emplace_back(idx);
        }

    GRBaddvars(d_cmodel,
               d_nSlacks,
               d_nSlacks,
               vbeg.data(),
               vbeg.data(),
               vval.data(),
               nullptr,
               nullptr,
               nullptr,
               nullptr,
               nullptr);

    GRBupdatemodel(d_cmodel);
}

MasterProblem::MasterProblem(MasterProblem const &other) :
    d_problem(other.d_problem), d_nSlacks(other.d_nSlacks)
{
    GRBupdatemodel(other.d_cmodel);
    d_cmodel = GRBcopymodel(other.d_cmodel);
}

MasterProblem::~MasterProblem()
{
    GRBfreemodel(d_cmodel);
}

void MasterProblem::addCut(Decomposition::Cut &cut)
{
    d_nSlacks++;

    GRBaddvar(d_cmodel,  // new slack variable
              0,
              nullptr,
              nullptr,
              0,
              0,
              arma::datum::inf,
              GRB_CONTINUOUS,
              nullptr);

    size_t const n1 = d_problem.Amat().n_rows;

    auto cind = arma::regspace<arma::Col<int>>(0, n1 + 2);
    cind[n1 + 1] = n1 + d_nSlacks;  // refers to the new slack variable.

    arma::vec cval(n1 + 2);
    cval.subvec(1, n1) = -cut.beta;
    cval[0] = 1;        // theta coefficient
    cval[n1 + 1] = -1;  // >= constraint, so slack features with -1

    GRBaddconstr(d_cmodel,
                 n1 + 2,
                 cind.memptr(),
                 cval.memptr(),
                 GRB_EQUAL,
                 cut.gamma,
                 nullptr);
}

std::unique_ptr<arma::vec> MasterProblem::solve(Decomposition &decomposition,
                                                double tol)
{
    while (true)
    {
        GRBoptimize(d_cmodel);

        size_t const n1 = d_problem.Amat().n_rows;

        auto xVals = std::make_unique<arma::vec>(n1);
        GRBgetdblattrarray(d_cmodel, "X", 1, n1, xVals->memptr());

        double theta;
        GRBgetdblattrelement(d_cmodel, "X", 0, &theta);

        auto cut = decomposition.computeCut(*xVals);

        // Is the proposed cut violated by the current solution?
        if (cut.gamma + arma::dot(*xVals, cut.beta) >= theta + tol)
            addCut(cut);
        else
            return xVals;
    }
}
