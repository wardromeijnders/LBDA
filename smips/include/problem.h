#ifndef PROBLEM_H
#define PROBLEM_H

#include <armadillo>
#include <gurobi_c++.h>
#include <iosfwd>


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

    arma::mat d_Amat;
    arma::mat d_Tmat;
    arma::mat d_Wmat;

    // Each column corresponds to a single scenario (omega).
    arma::mat d_scenarios;

    void initSub();  // initializes the subproblem, and sets rhs = 0. Called by
                     // evaluate() when evaluate is called for the first time.

    void clearSub();  // should be called if problem data changes

public:
    // TODO make these members private
    double d_L = 0;  // lb of Q - TODO is this a sensible default?

    // number of >= and <= constraints in the first and second stage
    size_t d_nFirstStageLeqConstraints;
    size_t d_nFirstStageGeqConstraints;

    size_t d_nSecondStageLeqConstraints;
    size_t d_nSecondStageGeqConstraints;

    arma::vec d_firstStageLowerBound;
    arma::vec d_firstStageUpperBound;
    arma::vec d_secondStageLowerBound;
    arma::vec d_secondStageUpperBound;

    arma::vec d_firstStageCoeffs;
    arma::vec d_secondStageCoeffs;

    arma::vec d_scenarioProbabilities;
    arma::vec d_firstStageRhs;

    Problem(GRBEnv &env);

    Problem(const Problem &other) = delete;

    ~Problem();

    void enforceCcr(double penalty);  // TODO: enforce CCR assumption

    void ssv95(size_t S,
               bool fs_continuous,
               bool ss_binary,
               bool standard_T = true);

    // evaluates cx + Q(x) (does not check feasibility)
    double evaluate(arma::vec const &x);

    [[nodiscard]] size_t nFirstStageIntVars() const;

    [[nodiscard]] size_t nSecondStageIntVars() const;

    [[nodiscard]] size_t nScenarios() const;

    [[nodiscard]] arma::mat const &Amat() const;
    [[nodiscard]] arma::mat &Amat();

    [[nodiscard]] arma::mat const &Wmat() const;

    [[nodiscard]] arma::mat const &Tmat() const;

    [[nodiscard]] arma::mat const &scenarios() const;
};

inline size_t Problem::nFirstStageIntVars() const
{
    return d_nFirstStageIntVars;
};

inline size_t Problem::nSecondStageIntVars() const
{
    return d_nSecondStageIntVars;
};

inline size_t Problem::nScenarios() const
{
    return d_scenarios.n_cols;
}

inline arma::mat const &Problem::Amat() const
{
    return d_Amat;
}

inline arma::mat &Problem::Amat()
{
    return d_Amat;
}

inline arma::mat const &Problem::Wmat() const
{
    return d_Wmat;
}

inline arma::mat const &Problem::Tmat() const
{
    return d_Tmat;
}

inline arma::mat const &Problem::scenarios() const
{
    return d_scenarios;
}

#endif