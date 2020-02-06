#include "data.h"

arma::mat Data::rand_gaus_mat(size_t nRows, size_t nCols, double mean, double sd)
{
    return mean + sd * arma::randn<arma::mat>(nRows, nCols);
}
