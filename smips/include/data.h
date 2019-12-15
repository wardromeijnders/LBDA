#ifndef DATA_H
#define DATA_H

#include <algorithm>
#include <random>
#include <vector>


class Data
{
public:
    Data(int seed = std::random_device{}());  // initializes d_engine with random
                                              // seed, unless seed is specified
    std::mt19937 d_engine;

    std::vector<double> rand_unif_vec(size_t size, int low, int high);
    std::vector<double> unif_real_vec(size_t size, double low, double high);
    std::vector<std::vector<double>> rand_unif_mat(size_t nRows,
                                                   size_t nCols,
                                                   int low,
                                                   int high);
    std::vector<std::vector<double>> rand_gaus_mat(size_t nRows,
                                                   size_t nCols,
                                                   double mean,
                                                   double sd);

    void append_minus_eye(std::vector<std::vector<double>> &matrix);
    void append_zeros(std::vector<std::vector<double>> &mat, size_t nZeros);
    void append_r(std::vector<double> &vector, double r, size_t n);
};

#endif
