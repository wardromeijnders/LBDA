#include "decompositions/loosebenders.h"

#include <algorithm>

double LooseBenders::computeGomory(size_t scenario,
                                   arma::vec &rhs,
                                   arma::Col<int> const &vBasis,
                                   arma::Col<int> const &cBasis)
{
    auto const &Wmat = d_problem.Wmat();

    // TODO overhaul this method
    std::vector<int> basis(Wmat.n_rows + Wmat.n_cols);
    std::copy(vBasis.memptr(), vBasis.memptr() + Wmat.n_rows, basis.begin());
    std::copy(cBasis.memptr(),
              cBasis.memptr() + Wmat.n_cols,
              basis.begin() + Wmat.n_rows);

    auto &visited = d_visited[scenario];
    auto iterator = std::find(visited.begin(), visited.end(), basis);

    if (iterator != visited.end())
    {
        // find index and retrieve corresponding objective value
        size_t idx = std::distance(visited.begin(), iterator);
        return d_objectives[scenario][idx];
    }

    update(rhs, vBasis, cBasis);
    auto const objective = solve();

    visited.emplace_back(basis);
    d_objectives[scenario].emplace_back(objective);

    return objective;
}
