#include "problem.h"

void Problem::setBounds(arma::vec &l1,
                        arma::vec &u1,
                        arma::vec &l2,
                        arma::vec &u2)
{
    d_l1 = l1;
    d_u1 = u1;
    d_l2 = l2;
    d_u2 = u2;
}
