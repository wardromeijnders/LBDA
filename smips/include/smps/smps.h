#ifndef SMIPS_SMPS_H
#define SMIPS_SMPS_H

#include <armadillo>
#include <string>

namespace smps
{
    class Smps
    {
        std::string d_name;
        std::string d_objName;
        arma::vec d_objCoeffs;
        arma::sp_mat d_core;
        arma::vec d_rhs;
        arma::Col<char> d_constrSenses;
        arma::Col<char> d_varTypes;
        arma::vec d_lowerBounds;
        arma::vec d_upperBounds;

        arma::umat d_stageOffsets;

        std::map<std::string, int> d_row2idx;
        std::map<std::string, int> d_col2idx;

    public:
        Smps() = default;

        void readSmps(std::string const &location);

        std::string const &name() const;

        bool setName(std::string const &name);

        bool addObjective(std::string const &name);

        bool addConstr(std::string const &name, char type);

        bool addCoeff(std::string const &constr,
                      std::string const &var,
                      double coeff);

        bool addRhs(std::string const &constr, double coeff);

        bool addLowerBound(std::string const &var, double bound);

        bool addUpperBound(std::string const &var, double bound);

        bool addStage(std::string const &constr, std::string const &var);

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
    };
}  // namespace smps

#endif  // SMIPS_SMPS_H
