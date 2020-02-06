#ifndef DATA_H
#define DATA_H

#include <armadillo>
#include <iosfwd>
#include <random>

class Data
{
public:
    Data(int seed = std::random_device{}());

    arma::vec rand_unif_vec(size_t size, int low, int high);

    arma::mat rand_unif_mat(size_t nRows, size_t nCols, int low, int high);

    arma::mat rand_gaus_mat(size_t nRows, size_t nCols, double mean, double sd);

    void append_minus_eye(std::vector<std::vector<double>> &matrix);

    void append_zeros(std::vector<std::vector<double>> &mat, size_t nZeros);

    void append_r(std::vector<double> &vector, double r, size_t n);
};

#endif
