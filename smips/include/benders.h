#ifndef BENDERS_H
#define BENDERS_H

#include "cuts/cut.h"
#include "masterproblem.h"

#include <armadillo>

class Benders  // TODO make obsolete - this can be a single solve() function.
{
    size_t d_nCuts;
    double d_runTime;

    MasterProblem d_master;

public:
    explicit Benders(MasterProblem &master);

    Benders(Benders const &other) = default;

    std::unique_ptr<arma::vec> solve(Cut &cut, double tol = 1e-4);
};

#endif
