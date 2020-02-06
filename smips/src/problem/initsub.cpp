#include "problem.h"

void Problem::initSub()
{
    char vTypes[d_n2];
    std::fill(vTypes, vTypes + d_p2, GRB_INTEGER);
    std::fill(vTypes + d_p2, vTypes + d_n2, GRB_CONTINUOUS);

    GRBVar *vars = d_sub.addVars(d_l2.memptr(),
                                 d_u2.memptr(),
                                 d_q.memptr(),
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

    GRBLinExpr Wy[d_m2];
    for (size_t conIdx = 0; conIdx != d_m2; ++conIdx)
        Wy[conIdx].addTerms(d_Wmat.colptr(conIdx), vars, d_n2);

    // add constraints
    d_constrs = d_sub.addConstrs(Wy, senses, rhs, nullptr, d_m2);
    d_sub_initialized = true;

    delete[] vars;
}