#include "decompositions/loosebenders.h"

#include "subproblem.h"

#include <algorithm>


LooseBenders::LooseBenders(GRBEnv &env,
                           Problem const &problem,
                           arma::vec const &alpha,
                           double timeLimit) :
    Decomposition(env, problem),
    d_alpha(alpha),
    d_visited(problem.nScenarios()),
    d_objectives(problem.nScenarios())
{
    auto const &Wmat = d_problem.Wmat();

    d_vars = d_model.addVars(d_problem.secondStageLowerBound().memptr(),
                             d_problem.secondStageUpperBound().memptr(),
                             problem.secondStageCoeffs().memptr(),
                             problem.secondStageVarTypes().memptr(),
                             nullptr,
                             Wmat.n_rows);


    GRBLinExpr lhs[Wmat.n_cols];
    for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
        lhs[iter.col()] += *iter * d_vars[iter.row()];

    auto const &senses = d_problem.secondStageConstrSenses();
    arma::vec rhs = arma::zeros(Wmat.n_cols);

    d_constrs = d_model.addConstrs(lhs,
                                   senses.memptr(),
                                   rhs.memptr(),
                                   nullptr,
                                   Wmat.n_cols);

    d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
    d_model.update();
}

LooseBenders::~LooseBenders()
{
    delete[] d_vars;
    delete[] d_constrs;
}

LooseBenders::Cut LooseBenders::computeCut(arma::vec const &x)
{
    auto const &Tmat = d_problem.Tmat();

    arma::vec Tx = (x.t() * Tmat).t();          // TODO simplify
    arma::vec dual = arma::zeros(Tmat.n_cols);  // decomposition coeffs

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        sub.update(omega - Tx);
        sub.solve();

        auto const info = sub.gomInfo();
        double const prob = d_problem.probability(scenario);

        // Gomory is lambda^T (omega - alpha) + psi(omega - alpha), so we add
        // lambda^T alpha.
        arma::vec rhs = omega - d_alpha;
        gamma += prob * computeGomory(scenario, rhs, info.vBasis, info.cBasis);
        gamma += prob * arma::dot(info.lambda, d_alpha);

        dual -= prob * info.lambda;
    }

    return Cut{Tmat * dual, gamma};
}

double LooseBenders::computeGomory(size_t scenario,
                                   arma::vec &rhs,
                                   arma::Col<int> const &vBasis,
                                   arma::Col<int> const &cBasis)
{
    auto const &Wmat = d_problem.Wmat();

    // TODO overhaul this method - make lookups more efficient?
    std::vector<int> basis(Wmat.n_rows + Wmat.n_cols);
    std::copy(vBasis.memptr(), vBasis.memptr() + Wmat.n_rows, basis.begin());
    std::copy(cBasis.memptr(),
              cBasis.memptr() + Wmat.n_cols,
              basis.begin() + Wmat.n_rows);

    auto &visited = d_visited[scenario];
    auto iterator = std::find(visited.begin(), visited.end(), basis);

    if (iterator != visited.end())
    {
        // Retrieve index and corresponding objective value
        size_t idx = std::distance(visited.begin(), iterator);
        return d_objectives[scenario][idx];
    }

    update(rhs, vBasis, cBasis);
    auto const objective = solve();

    visited.emplace_back(basis);
    d_objectives[scenario].emplace_back(objective);

    return objective;
}

void LooseBenders::update(arma::vec &rhs,
                          arma::Col<int> const &vBasis,
                          arma::Col<int> const &cBasis)
{
    auto const &senses = d_problem.secondStageConstrSenses();

    for (size_t idx = 0; idx != senses.size(); ++idx)
    {
        if (cBasis[idx] != GRB_BASIC || senses[idx] == GRB_EQUAL)
            continue;

        if (senses[idx] == GRB_LESS_EQUAL)
            rhs[idx] = arma::datum::inf;

        if (senses[idx] == GRB_GREATER_EQUAL)
            rhs[idx] = -arma::datum::inf;
    }

    auto const &Wmat = d_problem.Wmat();

    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);

    arma::vec lb = d_problem.secondStageLowerBound();
    arma::vec ub = d_problem.secondStageUpperBound();

    // Relax appropriate variable bounds if the bound is not tight.
    lb.elem(arma::find(vBasis != GRB_NONBASIC_LOWER)).fill(-arma::datum::inf);
    ub.elem(arma::find(vBasis != GRB_NONBASIC_UPPER)).fill(arma::datum::inf);

    d_model.set(GRB_DoubleAttr_LB, d_vars, lb.memptr(), Wmat.n_rows);
    d_model.set(GRB_DoubleAttr_UB, d_vars, ub.memptr(), Wmat.n_rows);
}
