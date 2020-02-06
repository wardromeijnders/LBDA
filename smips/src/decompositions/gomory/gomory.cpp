#include "decompositions/gomory.h"

Gomory::Gomory(GRBEnv &env, Problem const &problem) :
    Relaxation(env),
    d_m2(problem.d_m2),
    d_n2(problem.d_n2),
    d_ss_leq(problem.d_ss_leq),
    d_ss_geq(problem.d_ss_geq),
    d_l2(problem.d_l2.memptr()),
    d_u2(problem.d_u2.memptr())
{
    // variable types
    char vTypes[d_n2];
    std::fill(vTypes, vTypes + problem.d_p2, GRB_INTEGER);
    std::fill(vTypes + problem.d_p2, vTypes + d_n2, GRB_CONTINUOUS);

    d_vars = d_model.addVars(d_l2,
                             d_u2,
                             problem.d_q.memptr(),
                             vTypes,
                             nullptr,
                             d_n2);

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
        Wy[conIdx].addTerms(problem.d_Wmat.colptr(conIdx), d_vars, d_n2);

    // add constraints
    d_constrs = d_model.addConstrs(Wy, senses, rhs, nullptr, d_m2);
    d_model.update();
}
