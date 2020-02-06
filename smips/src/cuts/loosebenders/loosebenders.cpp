#include "cuts/loosebenders.h"


LooseBenders::LooseBenders(GRBEnv &env,
                           Problem const &problem,
                           arma::vec const &alpha,
                           double timeLimit) :
    Cut(env, problem),
    d_alpha(alpha),
    d_gomory(env, problem),
    d_visited(problem.d_S),
    d_objectives(problem.d_S)
{
    d_gomory.setTimeLimit(timeLimit);
}
