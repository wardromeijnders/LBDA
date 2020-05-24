#ifndef SMIPS_SMPS_H
#define SMIPS_SMPS_H

#include <armadillo>
#include <string>

class Smps
{
    std::string d_name;
    std::string d_objName;

    arma::sp_mat d_core;
    arma::vec d_objCoeffs;
    arma::vec d_rhs;

    std::map<std::string, int> d_row2idx;
    std::map<std::string, int> d_col2idx;

    void readCoreFile(std::string const &location);

    void readStochFile(std::string const &location);

    void readTimeFile(std::string const &location);

    bool isValid();

    std::string trim(std::string const &target);

public:
    Smps() = default;

    bool readSmps(std::string const &location);

    std::string const &name() const;

    void setName(std::string &name);

    void addObjective(std::string const &name);

    void addConstr(std::string const &name, char type);

    void addCoeff(std::string const &constr,
                  std::string const &var,
                  double coeff);

    void addRhs(std::string const &constr, double coeff);
};

#endif  // SMIPS_SMPS_H
