#ifndef ALD_H
#define ALD_H

#include <algorithm>
#include <iostream>
#include <cstdio>

#include "gurobi_c++.h"
#include "gurobi_c.h"

#include <math.h>

#include "../../../problem_data/problem.h"


using namespace std;

class Ald
{
  public:
    size_t d_n1, d_n2, d_p2, d_m2;
    vector<vector<double>> &d_Tmat;
    GRBmodel *d_model;
    
    Ald(GRBenv *env, Problem &problem);
    Ald(const Ald &other);
    ~Ald();
    void update(double *rhs);
    
    bool is_integer(double val);
    void gmi_cut(int row, double yval, double *xbar, size_t nConstrs, size_t nVars,
                 GRBsvec &e_i, GRBsvec &B_row, GRBsvec &tableau, 
                 vector<vector<double>> &Tmat, vector<double>& tau, 
                 double *coef_x, double *coef_y, double &coef_rhs, double &coef_eta);
    void add_cut(size_t nVars, size_t nCuts,
                  double *coef_x, double *coef_y, double coef_eta, double coef_rhs,
                  vector<vector<double>> &Tmat, vector<double> &tau, vector<double> &omega);
    void clean();
    
    struct AldCut 
    {
      double cons;
      double coef_eta;
      vector<double> coef_x;  
    };  
    AldCut solve(double *xbar, vector<double> omega, size_t maxRounds);  
  
};


#endif