#include "cuts/loosebenders.h"

#include <algorithm>

double LooseBenders::computeGomory(size_t s,
                                   arma::vec &rhs,
                                   arma::Col<int> const &vBasis,
                                   arma::Col<int> const &cBasis)
{
    // TODO overhaul this method
    std::vector<int> basis(d_problem.d_n2 + d_problem.d_m2);
    std::copy(vBasis.memptr(), vBasis.memptr() + d_problem.d_n2, basis.begin());
    std::copy(cBasis.memptr(),
              cBasis.memptr() + d_problem.d_m2,
              basis.begin() + d_problem.d_n2);

    auto &visited_bases = d_visited[s];

    auto it = std::find(visited_bases.begin(), visited_bases.end(), basis);

    if (it != visited_bases.end())  // visited before
    {
        // find index and retrieve corresponding objective value
        size_t idx = std::distance(visited_bases.begin(), it);
        return d_objectives[s][idx];
    }

    d_gomory.update(rhs.memptr(), vBasis.memptr(), cBasis.memptr());
    double gom_obj = d_gomory.solve();

    visited_bases.emplace_back(basis);
    d_objectives[s].emplace_back(gom_obj);

    return gom_obj;
}
