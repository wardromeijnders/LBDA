#ifndef PROBLEM_H
#define PROBLEM_H

#include <armadillo>
#include <iosfwd>

/**
 * ProblemData instance. Can be constructed from an SMPS file.
 */
class ProblemData
{
    arma::sp_mat d_Amat;  // first-stage constraints
    arma::sp_mat d_Tmat;  // linking constraints for the second-stage
    arma::sp_mat d_Wmat;  // second-stage constraints

    arma::mat d_scenarios;  // each column is a scenario right-hand side (omega)
    arma::vec d_scenarioProbabilities;

    arma::Col<char> d_firstStageConstrSenses;
    arma::Col<char> d_secondStageConstrSenses;

    arma::Col<char> d_firstStageVarTypes;
    arma::Col<char> d_secondStageVarTypes;

    arma::vec d_firstStageCoeffs;   // first-stage objective coefficients
    arma::vec d_secondStageCoeffs;  // second-stage objective coefficients

    arma::vec d_firstStageLowerBound;
    arma::vec d_firstStageUpperBound;

    arma::vec d_secondStageLowerBound;
    arma::vec d_secondStageUpperBound;

    arma::vec d_firstStageRhs;  // first-stage right-hand side

public:
    ProblemData(arma::sp_mat Amat,
                arma::sp_mat Tmat,
                arma::sp_mat Wmat,
                arma::mat scenarios,
                arma::vec scenarioProbabilities,
                arma::Col<char> firstStageConstrSenses,
                arma::Col<char> secondStageConstrSenses,
                arma::Col<char> firstStageVarTypes,
                arma::Col<char> secondStageVarTypes,
                arma::vec firstStageCoeffs,
                arma::vec secondStageCoeffs,
                arma::vec firstStageLowerBound,
                arma::vec firstStageUpperBound,
                arma::vec secondStageLowerBound,
                arma::vec secondStageUpperBound,
                arma::vec firstStageRhs);

    /**
     * Constructs a ProblemData instance from the passed-in SMPS file location.
     *
     * @param location SMPS file location (should not contain any extensions).
     * @return         ProblemData instance.
     */
    static ProblemData fromSmps(std::string const &location);

    arma::Col<char> const &firstStageConstrSenses() const
    {
        return d_firstStageConstrSenses;
    }

    arma::Col<char> const &secondStageConstrSenses() const
    {
        return d_secondStageConstrSenses;
    }

    arma::Col<char> const &firstStageVarTypes() const
    {
        return d_firstStageVarTypes;
    }

    arma::Col<char> const &secondStageVarTypes() const
    {
        return d_secondStageVarTypes;
    }

    /**
     * Sparse, first-stage constraint matrix. Each column specifies a
     * constraint, each row a variable.
     */
    arma::sp_mat const &Amat() const
    {
        return d_Amat;
    }

    /**
     * Sparse, second-stage linking matrix for the first-stage variables. Each
     * column specifies coefficients for a constraint, each row a variable.
     */
    arma::sp_mat const &Tmat() const
    {
        return d_Tmat;
    }

    /**
     * Sparse, second-stage constraint matrix. Each column specifies a
     * constraint, each row a variable.
     */
    arma::sp_mat const &Wmat() const
    {
        return d_Wmat;
    }

    arma::vec const &firstStageCoeffs() const
    {
        return d_firstStageCoeffs;
    }

    arma::vec const &secondStageCoeffs() const
    {
        return d_secondStageCoeffs;
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

    arma::vec const &firstStageRhs() const
    {
        return d_firstStageRhs;
    }

    size_t nScenarios() const
    {
        return d_scenarios.n_cols;
    }

    /**
     * Scenarios. Each column specifies a right-hand side (scenario).
     */
    arma::mat const &scenarios() const
    {
        return d_scenarios;
    }

    /**
     * Probability of a given scenario (index).
     */
    double scenarioProbability(size_t scenario) const
    {
        return d_scenarioProbabilities[scenario];
    }
};

#endif