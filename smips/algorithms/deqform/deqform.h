#ifndef DEQFORM_H
#define DEQFORM_H

#include "gurobi_c++.h"
#include <vector>
#include <algorithm>
#include <iostream>

#include "../../problem_data/problem.h"

using namespace std;

class DeqForm
{
  public:
    size_t d_n1;
    GRBModel d_model;  
    GRBVar *d_xVars;
    
    size_t d_status;  // has the model been solved? 
                      // 0: yes
                      // 1: no 
                      // 2: model is infeasible
    double* d_xVals;
    
    double d_objVal;
    double d_objBound;    
    
    bool d_MIP; // is the problem a MIP?
    double d_MIPGap;
    double d_runTime;
    
    DeqForm(GRBEnv &env, Problem &problem); // using the problem data , d_model is initialized
                                          // status is set to 1
                                          
    ~DeqForm();   // deletes vars (dynamically allocated to the heap blabla)                                  
    
    void init_fs(size_t n1, size_t p1, size_t m1, 
                 size_t fs_leq, size_t fs_geq,
                 double *lb, double *ub, 
                 double *c, double *rhs, 
                 vector<vector<double>> &Amat);
    void init_ss(size_t n1, size_t n2, size_t p2, size_t m2, size_t S, 
                 size_t ss_leq, size_t ss_geq, 
                 double *lb, double *ub,
                 double *probs, double *q,
                 vector<vector<double>> &Tmat, vector<vector<double>> &Wmat,
                 vector<vector<double>> &omega);    
    void solve(double time_limit = 1e20); // solves grbModel, updates status to 1 or 2, updates xvals, updates MIPGap  
};

#endif