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

    GRBaddvars(d_cmodel,  // first-stage (x) variables.
               Amat.n_rows,
               0,
               nullptr,
               nullptr,
               nullptr,
               d_problem.firstStageCoeffs().memptr(),
               d_problem.d_firstStageLowerBound.memptr(),
               d_problem.d_firstStageUpperBound.memptr(),
               d_problem.firstStageVarTypes().memptr(),
               nullptr);

    auto const &rhs = d_problem.firstStageRhs();
    auto const &senses = d_problem.firstStageConstrSenses();

    for (size_t constraint = 0; constraint != Amat.n_cols; ++constraint)
    {
        // TODO this is not that nice, I think
        auto const col = Amat.col(constraint);
        std::vector<int> ind;
        std::vector<double> val;

        for (auto it = Amat.begin_col(constraint);
             it != Amat.end_col(constraint);
             ++it)
        {
            ind.emplace_back(it.row() + 1);  // skip theta!
            val.emplace_back(*it);
        }

        GRBaddconstr(d_cmodel,
                     ind.size(),
                     ind.data(),
                     val.data(),
                     senses(constraint),
                     rhs(constraint),
                     nullptr);
    }

    GRBupdatemodel(d_cmodel);
}

MasterProblem::MasterProblem(MasterProblem const &other) :
    d_problem(other.d_problem)
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
    size_t const n1 = d_problem.Amat().n_rows;

    auto cind = arma::regspace<arma::Col<int>>(0, n1 + 1);

    arma::vec cval(n1 + 1);
    cval.subvec(1, n1) = -cut.beta;
    cval[0] = 1;  // theta coefficient

    GRBaddconstr(d_cmodel,
                 n1 + 1,
                 cind.memptr(),
                 cval.memptr(),
                 GRB_GREATER_EQUAL,
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
