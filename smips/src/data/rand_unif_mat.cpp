#include "data.h"

arma::mat Data::rand_unif_mat(size_t nRows, size_t nCols, int low, int high)
{
    return low + (high - low) * arma::randu(nRows, nCols);
}
