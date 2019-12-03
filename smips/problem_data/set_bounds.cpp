#include "problem.h"

void Problem::set_bounds(vector<double> &l1, vector<double> &u1, 
                         vector<double> &l2, vector<double> &u2)
{
  d_l1 = l1; d_u1 = u1; d_l2 = l2; d_u2 = u2;
}