#ifndef ZK_H
#define ZK_H

#include <algorithm>
#include <iostream>

#include "gurobi_c++.h"
#include "gurobi_c.h"

#include <math.h>

#include "../../../problem_data/problem.h"


using namespace std;

class ZK
{
  public:
    size_t d_n1, d_p1, d_n2, d_p2, d_nVars;
    
    int d_nConstrs;
    // cutting planes are of the form Wy == omega - T * x - tau * theta
    // y includes slacks. W is not stored.
    // we keep track for computing the rhs and for subgradient inequalities
    vector<double> d_omega;
    vector<vector<double>> d_Tmat;
    vector<double> d_tau;
    
    GRBmodel *d_model;
    ZK(GRBenv *env, Problem &problem, size_t scenario);
    ZK(const ZK &other);
    ZK(ZK &&other);
    ~ZK();
    
    
    void update(double *x, double theta);    // computes and updates rhs
    void solve(double *x, double theta, GRBmodel *master, vector<double> &kappa, vector<vector<double>> &beta, vector<double> &gamma, size_t maxRounds);
    void subgradient(double &alpha, double *beta, double &kappa, double weight);  // v >= alpha + beta^T x + tau * theta  
    void optimize();
    
    bool is_integer(double val, double precision = 1e-6);
    void compute_tab_row_x(double *tab_row_x, int nVarsMaster, int row, GRBmodel *master);
    void compute_tab_row_y(double *tab_row_y, int row);
    void gmi_cut(double *tab_row_x, double *tab_row_y, double a0, double *coef_x, double *coef_y, double &coef_theta, int nVarsMaster);
    void transform_cut(double *coef_x, double &coef_theta, double &coef_rhs, vector<double> &kappa, vector<vector<double>> &beta, vector<double> &gamma, size_t nSlacks);
    
       // computes the rhs (1 + coef_theta * theta + coef_x^T x), adds the cut, and store cut coeffients (omega, Tmat, and tau) for future calls to update()
    void add_cut(double *coef_x, double *coef_y, double coef_theta, double coef_rhs, double *x, double theta, size_t nxVars, size_t conIdx);   
    


  
};


#endif