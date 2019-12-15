#ifndef PSLP_H
#define PSLP_H

#include "master.h"
#include "problem.h"
#include "zk.h"

class Pslp
{
public:
    size_t d_n1;
    size_t d_S;
    size_t d_nCuts;

    GRBModel d_model;
    GRBVar d_z;
    GRBVar d_eta;
    std::vector<GRBConstr> d_constrs;

    double *d_x;
    double d_theta;

    // optimality cuts: eta + nu * z >= rhs = kappa^T x + gamma - nu * theta
    std::vector<std::vector<double>> d_kappa;
    std::vector<double> d_gamma;
    std::vector<double> d_nu;

    std::vector<ZK> d_zk;
    std::vector<double> &d_probs;

    Pslp(GRBEnv &env, GRBenv *c_env, Problem &problem);
    Pslp(const Pslp &other);
    ~Pslp() = default;

    void zk_cut(double *x,
                double theta,
                Master &master,
                double *beta,
                double &gamma,
                double &kappa,
                size_t maxRounds);  // standard ZK cut

    // strong ZK cut: calls l_shaped() to solve the pslp, then computes
    // subgradient using LP duality
    void strong_zk_cut(double *x,
                       double theta,
                       Master &master,
                       double *beta,
                       double &gamma,
                       double &kappa,
                       size_t maxRounds);

    void l_shaped(Master &master, size_t maxRounds, double tol = 1e-4);

    void update_rhs(double *x, double theta);

    void update_tau(double tau_bar);
};


#endif