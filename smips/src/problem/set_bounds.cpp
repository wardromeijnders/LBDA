#include "problem.h"

void Problem::set_bounds(std::vector<double> &l1,
                         std::vector<double> &u1,
                         std::vector<double> &l2,
                         std::vector<double> &u2)
{
    d_l1 = l1;
    d_u1 = u1;
    d_l2 = l2;
    d_u2 = u2;
}
