#include "cglp.h"

void Cglp::add_disjunction(double *x, double theta, double *beta, double tau, double gamma)
{
  double minus_beta[d_n1];
  for (size_t var = 0; var != d_n1; ++var)
    minus_beta[var] = -beta[var];
  
  for (size_t var = 0; var != d_n1; ++var)
  {
        // theta >= beta^T x + tau (x_i - \bar x_i) + gamma
            // pi_theta        
    GRBVar lambda = d_model.addVar(0.0, 1e20, 0.0, GRB_CONTINUOUS);
    d_lambda = lambda;
    d_model.chgCoeff(d_constrs_theta[2 * var], lambda, 1.0);

            // pi_x
    GRBVar lambda_vector[d_n1];
    fill_n(lambda_vector, d_n1, lambda);
    


    d_model.chgCoeffs(d_constrs_x[2 * var], lambda_vector, minus_beta, d_n1);
    d_model.chgCoeff(d_constrs_x[2 * var][var], lambda, minus_beta[var] - tau);
            // pi_0
    d_model.chgCoeff(d_constrs_cons[2 * var], lambda, gamma - tau * x[var]);

        

        // \bar x_i - x_i
            // pi_theta
    d_model.chgCoeff(d_constrs_theta[2 * var + 1], lambda, 1.0);
            // pi_x
    d_model.chgCoeffs(d_constrs_x[2 * var + 1], lambda_vector, minus_beta, d_n1);
    d_model.chgCoeff(d_constrs_x[2 * var + 1][var], lambda, minus_beta[var] + tau);
            // pi_0
    d_model.chgCoeff(d_constrs_cons[2 * var + 1], lambda, gamma + tau * x[var]);
  }

  
    // set coefficients of the pi variables  
  d_pi_theta.set(GRB_DoubleAttr_Obj, theta);
  d_model.set(GRB_DoubleAttr_Obj, d_pi_x, x, d_n1);
}