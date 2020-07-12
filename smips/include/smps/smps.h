#ifndef SMIPS_SMPS_H
#define SMIPS_SMPS_H

#include <armadillo>
#include <string>

namespace smps
{
    /**
     * SMPS class, to be populated with data read from the file system.
     */
    class Smps
    {
        struct ScenNode  // scenario node - data and a pointer-to-parent.
        {
            double probability;
            std::map<int, double> rhs;
        };

        std::string d_name;
        std::string d_objName;
        arma::vec d_objCoeffs;
        arma::sp_mat d_core;
        arma::vec d_rhs;
        arma::Col<char> d_constrSenses;
        arma::Col<char> d_varTypes;
        arma::vec d_lowerBounds;
        arma::vec d_upperBounds;

        // Constraint index to vector of (value, prob) pairs. (INDEP)
        std::map<int, std::vector<std::pair<double, double>>> d_indep;

        // Vector of scenarios. (SCENARIOS)
        std::vector<ScenNode> d_scenarios;
        std::map<std::string, int> d_scen2idx;

        arma::umat d_stageOffsets;

        std::map<std::string, int> d_constr2idx;
        std::map<std::string, int> d_var2idx;

        arma::mat genIndepScenarios();

        arma::vec indepScenProbabilities();

        arma::mat genScenarios();

        arma::vec scenProbabilities();

    public:
        Smps() = default;

        /**
         * Reads (smps) data from the given file location, by parsing the three
         * files (.cor, .tim, and .sto) expected to be there. Once this function
         * returns, this object may be queried for the data.
         *
         * @throws std::runtime_error when one of the three files does not exist.
         *
         * @param location Location of the smps files.
         */
        void read(std::string const &location);

        std::string const &name() const
        {
            return d_name;
        }

        bool setName(std::string const &name)
        {
            d_name = name;
            return true;
        }

        bool addObjective(std::string const &name);

        bool addConstr(std::string const &name, char type);

        bool addCoeff(std::string const &constr,
                      std::string const &var,
                      double coeff,
                      char varType);

        bool addRhs(std::string const &constr, double coeff);

        bool addLowerBound(std::string const &var, double bound);

        bool addUpperBound(std::string const &var, double bound);

        bool addStage(std::string const &constr, std::string const &var);

        bool addIndep(std::string const &constr,
                      std::pair<double, double> value);

        bool addScenario(std::string const &scenario, double probability);

        bool addScenarioRealisation(std::string const &scenario,
                                    std::string const &constr,
                                    double value);

        bool addVarType(std::string const &var, char type)
        {
            d_varTypes(d_var2idx[var]) = type;
            return true;
        }

        arma::sp_mat Amat();

        arma::sp_mat Tmat();

        arma::sp_mat Wmat();

        arma::vec firstStageObjCoeffs();

        arma::vec secondStageObjCoeffs();

        arma::Col<char> firstStageConstrSenses();

        arma::Col<char> secondStageConstrSenses();

        arma::Col<char> firstStageVarTypes();

        arma::Col<char> secondStageVarTypes();

        arma::vec firstStageLowerBound();

        arma::vec firstStageUpperBound();

        arma::vec secondStageLowerBound();

        arma::vec secondStageUpperBound();

        arma::vec firstStageRhs();

        /**
         * Base RHS for the second stage. Note that this is not the scenario
         * specific RHS, which should be obtained via
         * <code>generateScenarios()</code>.
         */
        arma::vec secondStageRhs();

        arma::mat generateScenarios();

        arma::vec scenarioProbabilities();
    };
}  // namespace smps

#endif  // SMIPS_SMPS_H
