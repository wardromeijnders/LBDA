#ifndef LOOSEBENDERS_H
#define LOOSEBENDERS_H

#include "cutfamily.h"


class LooseBenders : public CutFamily
{
    arma::vec const d_alpha;

    GRBConstr *d_constrs;
    GRBVar *d_vars;

    // For each scenario, we store the basis matrices that we have visited
    // (encoded by vBasis, cBasis).
    std::vector<std::vector<std::vector<int>>> d_visited;

    // For each visited basis matrix, we store the corresponding Gomory
    // objective value.
    std::vector<std::vector<double>> d_objectives;

    double computeGomory(size_t scenario,
                         arma::vec &rhs,
                         arma::Col<int> const &vBasis,
                         arma::Col<int> const &cBasis);

    void update(arma::vec &rhs,
                arma::Col<int> const &vBasis,
                arma::Col<int> const &cBasis);

public:
    LooseBenders(ProblemData const &problem,
                 arma::vec const alpha,
                 double timeLimit = 1e2);

    ~LooseBenders() override;

    CutFamily::Cut computeCut(arma::vec const &x) override;
};

#endif  // LOOSEBENDERS_H
