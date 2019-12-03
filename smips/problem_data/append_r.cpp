#include "data.h"

void Data::append_r(vector<double> &vector, double r, size_t n)
{
  for (size_t idx = 0; idx != n; ++idx)
    vector.push_back(r);
}