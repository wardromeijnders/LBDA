#ifndef PROBLEM_H
#define PROBLEM_H

#include "data.h"

#include <gurobi_c++.h>
#include <iosfwd>

// TODO: enforce CCR assumption

class Problem
{
    Data d_gen;  // used to generate (random) data

    GRBModel d_sub;  // subproblem, used for evaluation of cx + Q(x) (more
                     // precisely, of v(omega, x)).

    // useful to quickly update rhs of d_sub (heap allocated)
    GRBConstr *d_constrs;

    // is d_sub initialized. (destructor will only call delete[] on d_constrs
    // if yes)
    bool d_sub_initialized;

    void initSub();  // initializes the subproblem, and sets rhs = 0. Called by
                     // evaluate() when evaluate is called for the first time.

    void clearSub();  // should be called if problem data changes

public:
    // TODO make these members private
    // size parameters
    size_t d_m1;
    size_t d_m2;
    size_t d_n1;
    size_t d_n2;
    size_t d_p1;
    size_t d_p2;
    size_t d_q1;  // TODO unused?
    size_t d_q2;
    size_t d_S;

    double d_L;  // lb of Q

    // number of >= and <= constraints in the first and second stage
    size_t d_fs_leq;
    size_t d_fs_geq;
    size_t d_ss_leq;
    size_t d_ss_geq;

    std::vector<double> d_l1;
    std::vector<double> d_u1;
    std::vector<double> d_l2;
    std::vector<double> d_u2;

    std::vector<double> d_c;
    std::vector<double> d_b;
    std::vector<double> d_q;

    std::vector<std::vector<double>> d_Amat;
    std::vector<std::vector<double>> d_Tmat;
    std::vector<std::vector<double>> d_Wmat;

    // rows of d_omega correspond to scenarios
    std::vector<std::vector<double>> d_omega;
    std::vector<double> d_probs;

    Problem(Data &generator, GRBEnv &env);

    Problem(size_t n1,
            size_t p1,
            size_t m1,
            size_t n2,
            size_t p2,
            size_t m2,
            size_t S,
            Data &generator,
            GRBEnv &env,
            size_t fs_leq = 0,
            size_t fs_geq = 0,
            size_t ss_leq = 0,
            size_t ss_geq = 0);

    Problem(const Problem &other) = delete;

    ~Problem();

    // Initializes A, T, W, b, c, q (>= constraints)
    void randomInstance(int A_low = 1,
                        int A_high = 6,
                        int T_low = 1,
                        int T_high = 6,
                        int W_low = 1,
                        int W_high = 6,
                        int c_low = 1,
                        int c_high = 5,
                        int b_low = 1,
                        int b_high = 5,
                        int q_low = 5,
                        int q_high = 10);

    void enforceCcr(double penalty);

    // initializes d_omega
    void setGaussianOmega(double mean, double sd);

    void setBounds(std::vector<double> &l1,
                   std::vector<double> &u1,
                   std::vector<double> &l2,
                   std::vector<double> &u2);

    void sizes(size_t S);

    void ssv95(size_t S,
               bool fs_continuous,
               bool ss_binary,
               bool standard_T = true);

    void sslp(size_t nServers, size_t nClients, size_t S);

    void classic_ri();

    // evaluates cx + Q(x) (does not check feasibility)
    double evaluate(double const *x);
};

#endif