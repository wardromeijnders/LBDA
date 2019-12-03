#include "data.h"

void Data::append_minus_eye(vector<vector<double>> &matrix)
{
  size_t nRows = matrix.size();
  size_t nCols = matrix[0].size();
  
  for (size_t row_idx  = 0; row_idx != nRows; ++row_idx)
  {
    vector<double> &row = matrix[row_idx];
    for (size_t col_idx = 0; col_idx != nRows; ++col_idx)
      row.push_back(0.0);
      
    row[nCols + row_idx] = -1.0;
    
  }
}