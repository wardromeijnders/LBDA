#include "data.h"

std::vector<double> Data::rand_unif_vec(size_t size, int low, int high)
{
    std::uniform_int_distribution<int> uni(low, high);
    std::vector<double> vec(size);  // initialize
                                    // and fill element-wise

    for (size_t idx = 0; idx != size; ++idx)
        vec[idx] = uni(d_engine);

    return vec;
}
