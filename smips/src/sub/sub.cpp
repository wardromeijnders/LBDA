#include "sub.h"

Sub::Sub(GRBEnv &env, Problem &problem) :
    d_m2(problem.d_m2),
    d_n2(problem.d_n2),
    d_q(problem.d_q),
    d_model(env)
{
    size_t n2 = problem.d_n2;
    size_t m2 = problem.d_m2;

    size_t ss_leq = problem.d_ss_leq;
    size_t ss_geq = problem.d_ss_geq;

    // variable types
    char vTypes[n2];
    std::fill(vTypes, vTypes + n2, GRB_CONTINUOUS);

    // cost vector
    double *q = problem.d_q.data();  // transform cost vector and omega to
                                     // c-style array add variables
    d_vars = d_model.addVars(problem.d_l2.data(),
                             problem.d_u2.data(),
                             q,
                             vTypes,
                             NULL,
                             n2);

    // constraint senses
    char senses[m2];
    std::fill(senses, senses + ss_leq, GRB_LESS_EQUAL);
    std::fill(senses + ss_leq, senses + ss_leq + ss_geq, GRB_GREATER_EQUAL);
    std::fill(senses + ss_leq + ss_geq, senses + m2, GRB_EQUAL);

    // constraint rhs
    double rhs[m2];
    std::fill(rhs, rhs + m2, 0.0);

    // constraint lhs
    std::vector<std::vector<double>> &Wmat = problem.d_Wmat;
    GRBLinExpr Wy[m2];

    for (size_t conIdx = 0; conIdx != m2; ++conIdx)
    {
        double *row = Wmat[conIdx].data();
        Wy[conIdx].addTerms(row, d_vars, n2);
    }

    // add constraints
    d_constrs = d_model.addConstrs(Wy, senses, rhs, NULL, m2);
    d_model.update();
}