#include "data.h"

arma::vec Data::rand_unif_vec(size_t size, int low, int high)
{
    return low + (high - low) * arma::randu<arma::vec>(size);
}
