#ifndef LPDUAL_H
#define LPDUAL_H

#include "decomposition.h"


class LpDual : public Decomposition
{
public:
    LpDual(GRBEnv &env, Problem const &problem);

    Decomposition::Cut computeCut(arma::vec const &x) override;
};

#endif  // LPDUAL_H
