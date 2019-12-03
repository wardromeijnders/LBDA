#include "deqform.h"

void DeqForm::init_ss(size_t n1, size_t n2, size_t p2, size_t m2, size_t S, 
                      size_t ss_leq, size_t ss_geq, 
                      double *lb, double *ub,
                      double *probs, double *q,
                      vector<vector<double>> &Tmat, vector<vector<double>> &Wmat,
                      vector<vector<double>> &omega)
{
  GRBLinExpr Tx[m2];
  for (size_t conIdx = 0; conIdx != m2; ++conIdx)
  {
    double *row = Tmat[conIdx].data();
    Tx[conIdx].addTerms(row, d_xVars, n1);
  }
  
      // variable types    
  char vTypes2[n2];
  fill_n(vTypes2, p2, GRB_INTEGER);
  fill_n(vTypes2 + p2, n2 - p2, GRB_CONTINUOUS);    
  
  
      //constraint senses
  char senses2[m2];
  fill(senses2,                   senses2 + ss_leq,          GRB_LESS_EQUAL);
  fill(senses2 + ss_leq,          senses2 + ss_leq + ss_geq, GRB_GREATER_EQUAL);
  fill(senses2 + ss_leq + ss_geq, senses2 + m2,              GRB_EQUAL);
  
      // for each scenario: add variables and constraints
  double prob;
  for (size_t s = 0; s != S; ++s)
  {
    prob = probs[s];
    double costs[n2];
    for (size_t var = 0; var != n2; ++var)
      costs[var] = prob * q[var];
       
    double *rhsOmega = omega[s].data(); 
        // add variables
    GRBVar *yVars = d_model.addVars(lb, ub, costs, vTypes2, NULL, n2); 
    
        // lhs expression of second-stage constraints, Wy will be added in a loop
    GRBLinExpr TxWy[m2] = Tx;
    for (size_t conIdx = 0; conIdx != m2; ++conIdx)
    {
      double *row = Wmat[conIdx].data();      
      TxWy[conIdx].addTerms(row, yVars, n2);
    }
        // add constraints
    GRBConstr *constrs = d_model.addConstrs(TxWy, senses2, rhsOmega, NULL, m2);
    delete[] yVars;
    delete[] constrs;
  }   
}