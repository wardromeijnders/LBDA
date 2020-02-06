#ifndef STRONGBENDERS_H
#define STRONGBENDERS_H

#include "cut.h"
#include "decompositions/lagrangian.h"


class StrongBenders : public Cut
{
    Lagrangian d_lr;

public:
    StrongBenders(GRBEnv &env, Problem const &problem);

    Cut::CutResult computeCut(arma::vec const &x) override;
};

#endif  // STRONGBENDERS_H
