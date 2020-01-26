#include "benders.h"

#include <algorithm>

double Benders::computeGomory(
    size_t s, int *vBasis, int *cBasis, double const *ws, double const *alpha)
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

    double rhs[d_m2];

    for (size_t row = 0; row != d_m2; ++row)
        rhs[row] = ws[row] - alpha[row];

    d_gomory.update(rhs, vBasis, cBasis);
    double gom_obj = d_gomory.solve();

    visited_bases.emplace_back(basis);
    d_objectives[s].emplace_back(gom_obj);

    return gom_obj;
}
