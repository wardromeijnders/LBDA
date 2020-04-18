#include "main.h"

#include <cassert>

int main(int argc, char **argv)
{
    // TODO check all output numbers (verification).
    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    env.set(GRB_IntParam_Threads, 1);

    GRBenv *c_env;
    GRBloadenv(&c_env, nullptr);
    GRBsetintparam(c_env, "OutputFlag", 0);
    GRBsetintparam(c_env, "Threads", 1);

    Problem problem(env);
    problem.ssv95(11, true, true, true);

    DeterministicEquivalent detEqv(env, problem);
    auto ptr = detEqv.solve(900.0);
    auto res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';
    assert(std::abs(problem.evaluate(res)) - 50.814 <= 0.001);

    MasterProblem master{c_env, problem};

    LpDual lpCut{env, problem};
    ptr = master.solve(lpCut);
    res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';
    assert(std::abs(problem.evaluate(res)) - 59.8893 <= 0.001);

    arma::vec alpha = arma::zeros(problem.Tmat().n_cols);

    LooseBenders lbdaCut{env, problem, alpha, 1.0};
    auto master2 = MasterProblem(master);
    ptr = master2.solve(lbdaCut);
    res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';
    assert(std::abs(problem.evaluate(res)) - 60.9787 <= 0.001);

    StrongBenders sbCut{env, problem};
    auto master3 = MasterProblem(master);
    ptr = master3.solve(sbCut);
    res = *ptr;

    std::cout << res;
    std::cout << "\ncx + Q(x) = " << problem.evaluate(res) << '\n';
    assert(std::abs(problem.evaluate(res)) - 59.8893 <= 0.001);

    GRBfreeenv(c_env);
}
