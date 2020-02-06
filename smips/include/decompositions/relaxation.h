#ifndef RELAXATION_H
#define RELAXATION_H

#include <gurobi_c++.h>


class Relaxation  // TODO think about name
{
protected:
    GRBModel d_model;

public:
    explicit Relaxation(GRBEnv &env);

    Relaxation(Relaxation const &other) = default;

    virtual ~Relaxation() = default;

    virtual double solve();
};

#endif  // RELAXATION_H
