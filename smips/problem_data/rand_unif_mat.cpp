#include "data.h"

vector<vector<double>> Data::rand_unif_mat(size_t nRows, size_t nCols, int low, int high)
{
  uniform_int_distribution<int> uni(low, high);
  vector<vector<double>> mat(nRows);  // initialize
                                      // and fill element-wise  
  for (size_t row = 0; row != nRows; ++row)
  {
    for (size_t col = 0; col != nCols; ++col)
      mat[row].push_back(uni(d_engine));
  }
    
  return mat;
}