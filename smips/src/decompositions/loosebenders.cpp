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

    arma::Col<char> vTypes(Wmat.n_rows);
    vTypes.head(problem.nSecondStageIntVars()).fill(GRB_INTEGER);
    vTypes.tail(Wmat.n_rows - problem.nSecondStageIntVars()).fill(GRB_CONTINUOUS);

    d_vars = d_model.addVars(d_problem.d_secondStageLowerBound.memptr(),
                             d_problem.d_secondStageUpperBound.memptr(),
                             problem.d_secondStageCoeffs.memptr(),
                             vTypes.memptr(),
                             nullptr,
                             Wmat.n_rows);

    arma::Col<char> senses(Wmat.n_cols);
    senses.fill(GRB_GREATER_EQUAL);
    senses.head(d_problem.d_nSecondStageLeqConstraints).fill(GRB_LESS_EQUAL);
    senses
        .tail(Wmat.n_cols - d_problem.d_nSecondStageLeqConstraints
              - d_problem.d_nSecondStageGeqConstraints)
        .fill(GRB_EQUAL);

    GRBLinExpr lhs[Wmat.n_cols];
    for (auto iter = Wmat.begin(); iter != Wmat.end(); ++iter)
        lhs[iter.col()] += *iter * d_vars[iter.row()];

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

    arma::vec Tx = Tmat * x;
    arma::vec dual = arma::zeros(Tmat.n_cols);  // decomposition coeffs

    auto sub = SubProblem(d_env, d_problem);

    double gamma = 0;

    for (size_t scenario = 0; scenario != d_problem.nScenarios(); ++scenario)
    {
        arma::vec omega = d_problem.scenarios().col(scenario);

        sub.update(omega - Tx);
        sub.solve();

        auto const info = sub.gomInfo();
        double const prob = d_problem.d_scenarioProbabilities[scenario];

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
    size_t const ss_leq = d_problem.d_nSecondStageLeqConstraints;
    size_t const ss_geq = d_problem.d_nSecondStageGeqConstraints;

    // Relax <= and >= constraints if they are non-binding.
    rhs(arma::find(cBasis.head(ss_leq) == GRB_BASIC)).fill(arma::datum::inf);

    // TODO clean this up
    // >= constraints
    for (size_t con = ss_leq; con != ss_leq + ss_geq; ++con)
        if (cBasis[con] == GRB_BASIC)
            rhs[con] = -arma::datum::inf;

    auto const &Wmat = d_problem.Wmat();

    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);

    arma::vec lb = d_problem.d_secondStageLowerBound;
    arma::vec ub = d_problem.d_secondStageUpperBound;

    // Relax appropriate variable bounds if the bound is not tight.
    lb.elem(arma::find(vBasis != GRB_NONBASIC_LOWER)).fill(-arma::datum::inf);
    ub.elem(arma::find(vBasis != GRB_NONBASIC_UPPER)).fill(arma::datum::inf);

    d_model.set(GRB_DoubleAttr_LB, d_vars, lb.memptr(), Wmat.n_rows);
    d_model.set(GRB_DoubleAttr_UB, d_vars, ub.memptr(), Wmat.n_rows);
}
