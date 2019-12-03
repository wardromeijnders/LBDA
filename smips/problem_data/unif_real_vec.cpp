#include "data.h"

vector<double> Data::unif_real_vec(size_t size, double low, double high)
{
  uniform_real_distribution<double> uni(low, high);
  vector<double> vec(size); // initialize
                        // and fill element-wise  
  for (size_t idx = 0; idx != size; ++idx)
    vec[idx] = uni(d_engine);
    
  return vec;
}