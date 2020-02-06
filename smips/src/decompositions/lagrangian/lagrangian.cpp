#include "decompositions/lagrangian.h"

Lagrangian::Lagrangian(GRBEnv &env, Problem const &problem) :
    Relaxation(env),
    d_m2(problem.d_m2),
    d_n1(problem.d_n1),
    d_n2(problem.d_n2)
{
    // adding first-stage variables (z)
    char zTypes[d_n1];
    std::fill_n(zTypes, problem.d_p1, GRB_INTEGER);
    std::fill_n(zTypes + problem.d_p1, d_n1 - problem.d_p1, GRB_CONTINUOUS);

    d_z_vars = d_model.addVars(problem.d_l1.memptr(),
                               problem.d_u1.memptr(),
                               nullptr,
                               zTypes,
                               nullptr,
                               d_n1);  // cost coeffs set by update()

    // TODO: include first-stage constraints

    // adding second-stage variables (y)
    // variable types
    char yTypes[d_n2];
    std::fill_n(yTypes, problem.d_p2, GRB_INTEGER);
    std::fill_n(yTypes + problem.d_p2, d_n2 - problem.d_p2, GRB_CONTINUOUS);

    GRBVar *y_vars = d_model.addVars(problem.d_l2.memptr(),
                                     problem.d_u2.memptr(),
                                     problem.d_q.memptr(),
                                     yTypes,
                                     nullptr,
                                     d_n2);

    size_t ss_leq = problem.d_ss_leq;
    size_t ss_geq = problem.d_ss_geq;

    // constraint senses
    char senses[d_m2];
    std::fill(senses, senses + ss_leq, GRB_LESS_EQUAL);
    std::fill(senses + ss_leq, senses + ss_leq + ss_geq, GRB_GREATER_EQUAL);
    std::fill(senses + ss_leq + ss_geq, senses + d_m2, GRB_EQUAL);

    // constraint rhs
    double rhs[d_m2];
    std::fill(rhs, rhs + d_m2, 0.0);

    // constraint lhs
    GRBLinExpr TxWy[d_m2];

    for (size_t conIdx = 0; conIdx != d_m2; ++conIdx)
    {
        TxWy[conIdx].addTerms(problem.d_Tmat.colptr(conIdx), d_z_vars, d_n1);
        TxWy[conIdx].addTerms(problem.d_Wmat.colptr(conIdx), y_vars, d_n2);
    }

    // add constraints
    d_constrs = d_model.addConstrs(TxWy, senses, rhs, nullptr, d_m2);
    d_model.update();

    delete[] y_vars;
}
