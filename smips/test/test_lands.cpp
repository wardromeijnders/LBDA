#include "cutfamilies/lpdual.h"
#include "masterproblem.h"
#include "problem.h"

#include "gtest/gtest.h"


TEST(TestDataLandS, LpDual)
{
    auto problem = Problem::fromSmps("data/electric/lands");
    GRBEnv env;

    MasterProblem master(env, problem);

    LpDual family(env, problem);
    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), 397.75, 0.01);  // objective
    ASSERT_NEAR(x[0], 3.17, 0.01);                  // first-stage decisions
    ASSERT_NEAR(x[1], 5, 0.01);
    ASSERT_NEAR(x[2], 1.83, 0.01);
    ASSERT_NEAR(x[3], 4, 0.01);
}

TEST(TestDataLandS, LooseBenders)
{
    // TODO
}

TEST(TestDataLandS, StrongBenders)
{
    // TODO
}
