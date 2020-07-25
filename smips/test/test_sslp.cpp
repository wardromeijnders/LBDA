#include "cutfamilies/loosebenders.h"
#include "cutfamilies/lpdual.h"
#include "cutfamilies/strongbenders.h"
#include "masterproblem.h"
#include "problem.h"

#include "gtest/gtest.h"

/**
 * SSLP test data sets from SIPLIB, here:
 * https://www2.isye.gatech.edu/~sahmed/siplib/sslp/sslp.html.
 */


// TODO deduplicate some of this

TEST(TestDataSSLP_5_25_50, LpDual)
{
    auto problem = Problem::fromSmps("data/sslp/sslp_5_25_50");
    GRBEnv env;

    MasterProblem master(env, problem, -500, 0);

    LpDual family(env, problem);
    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), -121.6, 0.01);  // objective
    ASSERT_EQ(x[0], 1);                             // first-stage decisions
    ASSERT_EQ(x[1], 0);
    ASSERT_EQ(x[2], 1);
    ASSERT_EQ(x[3], 0);
    ASSERT_EQ(x[4], 0);
}

TEST(TestDataSSLP_5_25_50, StrongBenders)
{
    auto problem = Problem::fromSmps("data/sslp/sslp_5_25_50");
    GRBEnv env;

    MasterProblem master(env, problem, -500, 0);

    StrongBenders family(env, problem);
    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), -121.6, 0.01);  // objective
    ASSERT_EQ(x[0], 1);                             // first-stage decisions
    ASSERT_EQ(x[1], 0);
    ASSERT_EQ(x[2], 1);
    ASSERT_EQ(x[3], 0);
    ASSERT_EQ(x[4], 0);
}

TEST(TestDataSSLP_5_25_50, LooseBenders)
{
    auto problem = Problem::fromSmps("data/sslp/sslp_5_25_50");
    GRBEnv env;

    MasterProblem master(env, problem, -500, 0);

    arma::vec alpha = arma::zeros(problem.Wmat().n_cols);
    LooseBenders family(env, problem, alpha);

    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), -121.6, 0.01);  // objective
    ASSERT_EQ(x[0], 1);                             // first-stage decisions
    ASSERT_EQ(x[1], 0);
    ASSERT_EQ(x[2], 1);
    ASSERT_EQ(x[3], 0);
    ASSERT_EQ(x[4], 0);
}


TEST(TestDataSSLP_10_50_50, StrongBenders)
{
    auto problem = Problem::fromSmps("data/sslp/sslp_10_50_50");
    GRBEnv env;

    MasterProblem master(env, problem, -500, 0);

    StrongBenders family(env, problem);
    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), -364.76, 0.01);  // objective

    // TODO find a better way for vector comparisons
    ASSERT_EQ(x[0], 1);  // first-stage decisions
    ASSERT_EQ(x[1], 0);
    ASSERT_EQ(x[2], 0);
    ASSERT_EQ(x[3], 0);
    ASSERT_EQ(x[4], 1);
    ASSERT_EQ(x[5], 0);
    ASSERT_EQ(x[6], 1);
    ASSERT_EQ(x[7], 0);
    ASSERT_EQ(x[8], 0);
    ASSERT_EQ(x[9], 0);
}

// TODO larger instances?
