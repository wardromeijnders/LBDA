#include "deqform.h"

DeqForm::DeqForm(GRBEnv &env, Problem &problem) :
    d_n1(problem.d_n1),
    d_model(GRBModel(env)),
    d_status(status::UNSOLVED),
    d_isMip(problem.d_p1 != 0 && problem.d_p2 != 0)
{
    initFirstStage(d_n1,
                   problem.d_p1,
                   problem.d_m1,
                   problem.d_fs_leq,
                   problem.d_fs_geq,
                   problem.d_l1.data(),
                   problem.d_u1.data(),
                   problem.d_c.data(),
                   problem.d_b.data(),
                   problem.d_Amat);

    initSecondStage(d_n1,
                    problem.d_n2,
                    problem.d_p2,
                    problem.d_m2,
                    problem.d_S,
                    problem.d_ss_leq,
                    problem.d_ss_geq,
                    problem.d_l2.data(),
                    problem.d_u2.data(),
                    problem.d_probs.data(),
                    problem.d_q.data(),
                    problem.d_Tmat,
                    problem.d_Wmat,
                    problem.d_omega);
}
