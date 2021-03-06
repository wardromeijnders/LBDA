#include "cutfamilies/loosebenders.h"
#include "cutfamilies/lpdual.h"
#include "cutfamilies/strongbenders.h"
#include "masterproblem.h"
#include "problemdata.h"

#include "gtest/gtest.h"

/**
 * Modified 2-stage electricity problem of Louveaux and Smeers (1988). The
 * expected outcomes are vetted.
 */

// TODO deduplicate some of this

TEST(TestDataLandS, LpDual)
{
    auto problem = ProblemData::fromSmps("data/electric/LandS");

    MasterProblem master(problem);
    LpDual family(problem);

    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), 397.75, 0.01);  // objective
    ASSERT_NEAR(x[0], 3.17, 0.01);                  // first-stage decisions
    ASSERT_NEAR(x[1], 5, 0.01);
    ASSERT_NEAR(x[2], 1.83, 0.01);
    ASSERT_NEAR(x[3], 4, 0.01);
}

TEST(TestDataLandS, StrongBenders)
{
    auto problem = ProblemData::fromSmps("data/electric/LandS");

    MasterProblem master(problem);
    StrongBenders family(problem);

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
    auto problem = ProblemData::fromSmps("data/electric/LandS");

    MasterProblem master(problem);

    arma::vec alpha = arma::zeros(problem.Wmat().n_cols);
    LooseBenders family(problem, alpha);

    auto const res = master.solveWith(family);
    auto const x = *res;

    ASSERT_NEAR(master.objective(), 397.75, 0.01);  // objective
    ASSERT_NEAR(x[0], 3.17, 0.01);                  // first-stage decisions
    ASSERT_NEAR(x[1], 5, 0.01);
    ASSERT_NEAR(x[2], 1.83, 0.01);
    ASSERT_NEAR(x[3], 4, 0.01);
}
