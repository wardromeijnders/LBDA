#ifndef CUT_H
#define CUT_H

#include "problemdata.h"
#include "subproblem.h"

#include <armadillo>
#include <gurobi_c++.h>

class CutFamily
{
protected:
    GRBEnv d_env = GRBEnv();

    GRBModel d_model;
    ProblemData const &d_problem;
    SubProblem d_sub;

    explicit CutFamily(ProblemData const &problem);

    virtual double solve();

public:
    struct Cut
    {
        arma::vec beta;
        double gamma;
    };

    virtual CutFamily::Cut computeCut(arma::vec const &x) = 0;

    virtual ~CutFamily() = default;
};

#endif  // CUT_H
