#ifndef LPDUAL_H
#define LPDUAL_H

#include "cutfamily.h"


class LpDual : public CutFamily
{
public:
    LpDual(ProblemData const &problem);

    CutFamily::Cut computeCut(arma::vec const &x) override;
};

#endif  // LPDUAL_H
