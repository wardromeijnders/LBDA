#include "data.h"

std::vector<double> Data::unif_real_vec(size_t size, double low, double high)
{
    std::uniform_real_distribution<double> uni(low, high);
    std::vector<double> vec(size);  // initialize
                                    // and fill element-wise

    for (size_t idx = 0; idx != size; ++idx)
        vec[idx] = uni(d_engine);

    return vec;
}
