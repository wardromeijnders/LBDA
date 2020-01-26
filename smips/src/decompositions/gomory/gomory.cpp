#include "decompositions/gomory.h"

Gomory::Gomory(GRBEnv &env, Problem &problem) :
    d_m2(problem.d_m2),
    d_n2(problem.d_n2),
    d_ss_leq(problem.d_ss_leq),
    d_ss_geq(problem.d_ss_geq),
    d_l2(problem.d_l2.data()),
    d_u2(problem.d_u2.data()),
    d_model(env)
{
    // variable types
    char vTypes[d_n2];
    std::fill(vTypes, vTypes + problem.d_p2, GRB_INTEGER);
    std::fill(vTypes + problem.d_p2, vTypes + d_n2, GRB_CONTINUOUS);

    // cost vector
    double *q = problem.d_q.data();  // transform cost vector and omega to
                                     // c-style array add variables

    d_vars = d_model.addVars(d_l2, d_u2, q, vTypes, nullptr, d_n2);

    // constraint senses
    char senses[d_m2];
    std::fill(senses, senses + d_ss_leq, GRB_LESS_EQUAL);

    std::fill(senses + d_ss_leq,
              senses + d_ss_leq + d_ss_geq,
              GRB_GREATER_EQUAL);

    std::fill(senses + d_ss_leq + d_ss_geq, senses + d_m2, GRB_EQUAL);

    // constraint rhs
    double rhs[d_m2];
    std::fill(rhs, rhs + d_m2, 0.0);

    // constraint lhs
    GRBLinExpr Wy[d_m2];

    for (size_t conIdx = 0; conIdx != d_m2; ++conIdx)
    {
        double *row = problem.d_Wmat[conIdx].data();
        Wy[conIdx].addTerms(row, d_vars, d_n2);
    }

    // add constraints
    d_constrs = d_model.addConstrs(Wy, senses, rhs, nullptr, d_m2);
    d_model.update();
}
