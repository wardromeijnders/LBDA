#ifndef PSLP_H
#define PSLP_H

#include "../../../problem_data/problem.h"
#include "../master/master.h"
#include "../zk/zk.h"

class Pslp
{
  public:
    size_t d_n1, d_S;
    
    GRBModel d_model;
    GRBVar d_z;
    GRBVar d_eta;
    vector<GRBConstr> d_constrs;
    
    double *d_x, d_theta; 
    
        // optimality cuts: eta + nu * z >= rhs = kappa^T x + gamma - nu * theta 
    size_t d_nCuts;
    vector<vector<double>> d_kappa;
    vector<double> d_gamma;
    vector<double> d_nu;
 
    vector<ZK> d_zk;    
    vector<double> &d_probs;
    
    Pslp(GRBEnv &env, GRBenv *c_env, Problem &problem);
    Pslp(const Pslp &other);
    ~Pslp();
    
    void zk_cut(double *x, double theta, Master &master, double *beta, double &gamma, double &kappa, size_t maxRounds); // standard ZK cut
        // strong ZK cut: calls l_shaped() to solve the pslp, then computes subgradient using LP duality
    void strong_zk_cut(double *x, double theta, Master &master, double *beta, double &gamma, double &kappa, size_t maxRounds); 
    void l_shaped(Master &master, size_t maxRounds, double tol = 1e-4);
    
    
    
    void update_rhs(double *x, double theta);    
    void update_tau(double tau_bar);

};




#endif