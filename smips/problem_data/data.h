#ifndef DATA_H
#define DATA_H

#include <random>
#include <vector>
#include <algorithm>

using namespace std;

class Data
{
  public:
    Data(int seed = random_device{}()); // initializes d_engine with random seed, unless seed is specified
    std::mt19937 d_engine;
    
    vector<double> rand_unif_vec(size_t size, int low, int high);
    vector<double> unif_real_vec(size_t size, double low, double high);       
    vector<vector<double>> rand_unif_mat(size_t nRows, size_t nCols, int low, int high);  
    vector<vector<double>> rand_gaus_mat(size_t nRows, size_t nCols, double mean, double sd);   
    
    void append_minus_eye(vector<vector<double>> &matrix);
    void append_zeros(vector<vector<double>> &mat, size_t nZeros);
    void append_r(vector<double> &vector, double r, size_t n);
};

#endif