#include "masterproblem.h"

MasterProblem::MasterProblem(GRBEnv &env, Problem &problem) :
    d_problem(problem), d_model(env)
{
    // Theta
    d_model.addVar(d_problem.d_L, arma::datum::inf, 1.0, GRB_CONTINUOUS);

    auto &Amat = d_problem.Amat();
    auto *xVars = d_model.addVars(d_problem.d_firstStageLowerBound.memptr(),
                                  d_problem.d_firstStageUpperBound.memptr(),
                                  d_problem.firstStageCoeffs().memptr(),
                                  d_problem.firstStageVarTypes().memptr(),
                                  nullptr,
                                  Amat.n_rows);

    GRBLinExpr lhs[Amat.n_cols];

    for (auto iter = Amat.begin(); iter != Amat.end(); ++iter)
        lhs[iter.col()] += *iter * xVars[iter.row()];

    auto *constrs = d_model
                        .addConstrs(lhs,
                                    d_problem.firstStageConstrSenses().memptr(),
                                    d_problem.firstStageRhs().memptr(),
                                    nullptr,
                                    Amat.n_cols);

    delete[] xVars;
    delete[] constrs;

    d_model.update();
}

void MasterProblem::addCut(Decomposition::Cut &cut)
{
    size_t const n1 = d_problem.Amat().n_rows;

    GRBVar *vars = d_model.getVars();

    arma::vec cval(n1 + 1);
    cval[0] = 1;  // theta coefficient
    cval.subvec(1, n1) = -cut.beta;

    GRBLinExpr lhs;
    lhs.addTerms(cval.memptr(), vars, n1 + 1);

    delete[] vars;
    d_model.addConstr(lhs, GRB_GREATER_EQUAL, cut.gamma);
}

std::unique_ptr<arma::vec> MasterProblem::solve(Decomposition &decomposition,
                                                double tol)
{
    while (true)
    {
        d_model.optimize();

        size_t const numVars = d_problem.Amat().n_rows;

        auto *vars = d_model.getVars();

        auto *xValsPtr = d_model.get(GRB_DoubleAttr_X, vars + 1, numVars);
        double theta = vars[0].get(GRB_DoubleAttr_X);

        arma::vec xVals(xValsPtr, numVars);

        delete[] vars;
        delete[] xValsPtr;

        auto cut = decomposition.computeCut(xVals);

        // Is the proposed cut violated by the current solution?
        if (cut.gamma + arma::dot(xVals, cut.beta) >= theta + tol)
            addCut(cut);
        else
            return std::make_unique<arma::vec>(xVals);
    }
}
