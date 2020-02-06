#include "decompositions/cglp.h"

Cglp::Cglp(GRBEnv &env, MasterProblem &master) :
    d_n1(master.n1()),
    d_model(env),
    d_constrs_theta(2 * d_n1),
    d_constrs_x(2 * d_n1),
    d_constrs_cons(2 * d_n1)
{
    d_pi_theta = d_model.addVar(-1.0, 1.0, 0.0, GRB_CONTINUOUS);
    double lb[d_n1];
    double ub[d_n1];

    std::fill_n(lb, d_n1, -1.0);
    std::fill_n(ub, d_n1, 1.0);
    d_pi_x = d_model.addVars(lb, ub, nullptr, nullptr, nullptr, d_n1);
    d_pi_0 = d_model.addVar(-1.0, 1.0, -1.0, GRB_CONTINUOUS);

    auto const &Delta = master.cuts();
    size_t nCuts = Delta.size();

    double ones[nCuts];
    std::fill_n(ones, nCuts, 1.0);

    // each disjunctive term is of the form e theta >= Gamma x + Delta
    // corresponding multipliers are denoted by lambda
    // thus, pi_theta >= lambda^T e
    // pi_x,i >= -lambda^T Gamma_i (i-th column of gamma)
    // pi_0 <= lambda^T Delta

    auto const &Gamma = master.xCoeffs();

    for (size_t term = 0; term != 2 * d_n1; ++term)
    {
        GRBVar *lambda = d_model.addVars(nCuts);  // multipliers

        // constraint corresponding to pi_theta
        GRBLinExpr lambda_sum;
        lambda_sum.addTerms(ones, lambda, nCuts);
        d_constrs_theta[term] = d_model.addConstr(lambda_sum,
                                                  GRB_LESS_EQUAL,
                                                  d_pi_theta);

        // constraints corresponding to pi_x
        GRBLinExpr lhs[d_n1];
        for (size_t col = 0; col != d_n1; ++col)
        {
            double Gamma_column[nCuts];

            for (size_t row = 0; row != nCuts; ++row)
                Gamma_column[row] = Gamma[row][col];

            lhs[col] = GRBLinExpr(d_pi_x[col]);
            lhs[col].addTerms(Gamma_column, lambda, nCuts);
        }

        std::vector<char> senses(d_n1, GRB_GREATER_EQUAL);
        std::vector<double> rhs(d_n1, 0.);

        d_constrs_x[term] = d_model.addConstrs(lhs,
                                               senses.data(),
                                               rhs.data(),
                                               nullptr,
                                               d_n1);

        // constraint corresponding to pi_0
        GRBLinExpr lambda_delta;
        lambda_delta.addTerms(Delta.data(), lambda, nCuts);
        d_constrs_cons[term] = d_model.addConstr(lambda_delta,
                                                 GRB_GREATER_EQUAL,
                                                 d_pi_0);

        delete[] lambda;
    }
}
