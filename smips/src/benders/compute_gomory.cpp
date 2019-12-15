#include "benders.h"

#include <algorithm>

double Benders::compute_gomory(
    size_t s, int *vBasis, int *cBasis, double *ws, double *alpha)
{
    std::vector<double> basis(d_n2 + d_m2);
    std::copy(vBasis, vBasis + d_n2, basis.begin());
    std::copy(cBasis, cBasis + d_m2, basis.begin() + d_n2);

    auto &visited_bases = d_visited[s];

    auto it = std::find(visited_bases.begin(), visited_bases.end(), basis);

    if (it != visited_bases.end())  // visited before
    {
        // find index and retrieve corresponding objective value
        size_t idx = std::distance(visited_bases.begin(), it);
        return d_objectives[s][idx];
    }

    double omega_alpha[d_m2];  // rhs vector of gomory relaxation

    for (size_t row = 0; row != d_m2; ++row)  // compute element-by-element
        omega_alpha[row] = ws[row] - alpha[row];

    // update gomory relaxation
    d_gomory.update(omega_alpha, vBasis, cBasis);

    // solve gomory relaxation and store objective bound
    double gom_obj = d_gomory.solve();

    // basis is now visited: store it
    visited_bases.push_back(basis);

    // store corresponding objective value
    d_objectives[s].push_back(gom_obj);

    return gom_obj;
}
