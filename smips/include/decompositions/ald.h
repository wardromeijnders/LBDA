#ifndef ALD_H
#define ALD_H

#include "problem.h"

#include <algorithm>
#include <gurobi_c++.h>
#include <gurobi_c.h>


class Ald
{
public:
    size_t d_m2;
    size_t d_n1;
    size_t d_n2;
    size_t d_p2;

    std::vector<std::vector<double>> &d_Tmat;

    GRBmodel *d_model;

    Ald(GRBenv *env, Problem &problem);
    Ald(const Ald &other);
    ~Ald();

    void update(double *rhs);

    bool is_integer(double val);

    void gmi_cut(int row,
                 double yval,
                 double *xbar,
                 size_t nConstrs,
                 size_t nVars,
                 GRBsvec &e_i,
                 GRBsvec &B_row,
                 GRBsvec &tableau,
                 std::vector<std::vector<double>> &Tmat,
                 std::vector<double> &tau,
                 double *coef_x,
                 double *coef_y,
                 double &coef_rhs,
                 double &coef_eta);

    void add_cut(size_t nVars,
                 size_t nCuts,
                 double *coef_x,
                 double *coef_y,
                 double coef_eta,
                 double coef_rhs,
                 std::vector<std::vector<double>> &Tmat,
                 std::vector<double> &tau,
                 std::vector<double> &omega);

    void clean();

    struct AldCut
    {
        double cons;
        double coef_eta;
        std::vector<double> coef_x;
    };

    AldCut solve(double *xbar, std::vector<double> omega, size_t maxRounds);
};

#endif
