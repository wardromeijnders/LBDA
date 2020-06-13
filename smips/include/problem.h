#ifndef PROBLEM_H
#define PROBLEM_H

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>


// TODO this class is too big, and might need to be cut down a little
class Problem
{
    // TODO this is not the place - should move to the actual solver stuff?
    GRBModel d_sub;  // subproblem, used for evaluation of cx + Q(x) (more
                     // precisely, of v(omega, x)).

    // useful to quickly update rhs of d_sub (heap allocated)
    GRBConstr *d_constrs;

    bool d_isSubProblemInitialised = false;

    size_t d_nFirstStageIntVars = 0;  // TODO make constant!
    size_t d_nSecondStageIntVars = 0;

    arma::Col<char> d_firstStageConstrSenses;
    arma::Col<char> d_secondStageConstrSenses;

    arma::sp_mat d_Amat;
    arma::sp_mat d_Tmat;
    arma::sp_mat d_Wmat;

    // Each column corresponds to a single scenario (omega).
    arma::mat d_scenarios;

    void initSub();  // initializes the subproblem, and sets rhs = 0. Called by
                     // evaluate() when evaluate is called for the first time.

public:
    // TODO make these members private
    double d_L = 0;  // lb on Q - TODO is this a sensible default?

    arma::vec d_firstStageLowerBound;
    arma::vec d_firstStageUpperBound;
    arma::vec d_secondStageLowerBound;
    arma::vec d_secondStageUpperBound;

    arma::vec d_firstStageCoeffs;
    arma::vec d_secondStageCoeffs;

    arma::vec d_scenarioProbabilities;
    arma::vec d_firstStageRhs;

    Problem(GRBEnv &env);

    /**
     * Constructs a Problem instance from the passed-in SMPS file location.
     *
     * @param location SMPS file location (should not contain any extensions).
     * @param env      Gurobi environment in which to operate.
     * @return         Problem instance.
     */
    static Problem fromSmps(char const *location, GRBEnv &env);

    ~Problem();

    // evaluates cx + Q(x) (does not check feasibility)
    double evaluate(arma::vec const &x);

    size_t nFirstStageIntVars() const
    {
        return d_nFirstStageIntVars;
    }

    size_t nSecondStageIntVars() const
    {
        return d_nFirstStageIntVars;
    }

    arma::Col<char> const &firstStageConstrSenses() const
    {
        return d_firstStageConstrSenses;
    }

    arma::Col<char> const &secondStageConstrSenses() const
    {
        return d_secondStageConstrSenses;
    }

    size_t nScenarios() const
    {
        return d_scenarios.n_cols;
    }

    arma::sp_mat const &Amat() const
    {
        return d_Amat;
    }

    arma::sp_mat &Amat()
    {
        return d_Amat;
    }

    arma::sp_mat const &Wmat() const
    {
        return d_Wmat;
    }

    arma::sp_mat const &Tmat() const
    {
        return d_Tmat;
    }

    arma::mat const &scenarios() const
    {
        return d_scenarios;
    }
};

#endif