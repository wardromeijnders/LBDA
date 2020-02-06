#include "data.h"

Data::Data(int seed)
{
    arma::arma_rng::set_seed(seed);
}
