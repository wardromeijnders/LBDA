#include "decompositions/pslp.h"

Pslp::Pslp(GRBEnv &env, GRBenv *c_env, Problem &problem) :
    d_n1(problem.d_n1),
    d_S(problem.d_S),
    d_nCuts(0),
    d_model(env),
    d_probs(problem.d_probs)
{
    d_zk.reserve(d_S);

    for (size_t scenario = 0; scenario != d_S; ++scenario)
        d_zk.push_back(ZK{c_env, problem, scenario});

    d_z = d_model.addVar(0, 1e20, -1, GRB_CONTINUOUS);
    d_eta = d_model.addVar(0, 1e20, 1, GRB_CONTINUOUS);

    d_model.update();
}
