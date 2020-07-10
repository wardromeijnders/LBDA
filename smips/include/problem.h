#ifndef PROBLEM_H
#define PROBLEM_H

#include <armadillo>
#include <iosfwd>

/**
 * Problem instance. Can be constructed from an SMPS file.
 */
class Problem
{
    arma::Col<char> d_firstStageConstrSenses;
    arma::Col<char> d_secondStageConstrSenses;

    arma::Col<char> d_firstStageVarTypes;
    arma::Col<char> d_secondStageVarTypes;

    arma::sp_mat d_Amat;  // first-stage constraints
    arma::sp_mat d_Tmat;  // linking constraints for the second-stage
    arma::sp_mat d_Wmat;  // second-stage constraints

    arma::vec d_firstStageCoeffs;   // first-stage objective coefficients
    arma::vec d_secondStageCoeffs;  // second-stage objective coefficients

    arma::vec d_firstStageLowerBound;
    arma::vec d_firstStageUpperBound;

    arma::vec d_secondStageLowerBound;
    arma::vec d_secondStageUpperBound;

    arma::vec d_firstStageRhs;  // first-stage right-hand side

    arma::mat d_scenarios;  // each column is a scenario (omega)
    arma::vec d_scenarioProbabilities;

public:
    /**
     * Constructs a Problem instance from the passed-in SMPS file location.
     *
     * @param location SMPS file location (should not contain any extensions).
     * @return         Problem instance.
     */
    static Problem fromSmps(char const *location);

    arma::Col<char> const &firstStageConstrSenses() const
    {
        return d_firstStageConstrSenses;
    }

    arma::Col<char> const &secondStageConstrSenses() const
    {
        return d_secondStageConstrSenses;
    }

    arma::Col<char> &firstStageVarTypes()
    {
        return d_firstStageVarTypes;
    }

    arma::Col<char> const &firstStageVarTypes() const
    {
        return d_firstStageVarTypes;
    }

    arma::Col<char> const &secondStageVarTypes() const
    {
        return d_secondStageVarTypes;
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

    arma::vec &firstStageCoeffs()
    {
        return d_firstStageCoeffs;
    }

    arma::vec const &firstStageCoeffs() const
    {
        return d_firstStageCoeffs;
    }

    arma::vec const &secondStageCoeffs() const
    {
        return d_secondStageCoeffs;
    }

    arma::vec const &firstStageRhs() const
    {
        return d_firstStageRhs;
    }

    arma::vec const &firstStageLowerBound() const
    {
        return d_firstStageLowerBound;
    }

    arma::vec const &firstStageUpperBound() const
    {
        return d_firstStageUpperBound;
    }

    arma::vec const &secondStageLowerBound() const
    {
        return d_secondStageLowerBound;
    }

    arma::vec const &secondStageUpperBound() const
    {
        return d_secondStageUpperBound;
    }

    arma::mat const &scenarios() const
    {
        return d_scenarios;
    }

    double probability(size_t scenario) const
    {
        return d_scenarioProbabilities[scenario];
    }
};

#endif