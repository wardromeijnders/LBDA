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

        arma::umat d_stageOffsets;

        std::map<std::string, int> d_row2idx;
        std::map<std::string, int> d_col2idx;

        void readCoreFile(std::string const &location);

        void readStochFile(std::string const &location);

        void readTimeFile(std::string const &location);

    public:
        Smps() = default;

        void readSmps(std::string const &location);

        std::string const &name() const;

        bool setName(std::string &name);

        bool addObjective(std::string const &name);

        bool addConstr(std::string const &name, char type);

        bool addCoeff(std::string const &constr,
                      std::string const &var,
                      double coeff);

        bool addRhs(std::string const &constr, double coeff);

        bool addStage(std::string const &constr, std::string const &var);
    };

    std::string trim(std::string const &target);
}  // namespace smps

#endif  // SMIPS_SMPS_H
