#include "decompositions/pslp.h"

void Pslp::l_shaped(Master &master, size_t maxRounds, double tol)
{
    bool stop = false;
    size_t iter = 0;

    while (not stop)
    {
        ++iter;
        d_model.optimize();
        double zval = d_z.get(GRB_DoubleAttr_X);
        double etaval = d_eta.get(GRB_DoubleAttr_X);

        double nu = 0;
        double gamma = 0;
        double kappa[d_n1];
        std::fill_n(kappa, d_n1, 0);

        for (size_t scenario = 0; scenario != d_S; ++scenario)
        {
            ZK &sub = d_zk[scenario];
            sub.update(d_x, d_theta);  // change rhs
            sub.solve(d_x,
                      d_theta,
                      master.d_cmodel,
                      master.d_kappa,
                      master.d_beta,
                      master.d_gamma,
                      maxRounds);             // generate cutting planes
            sub.update(d_x, d_theta + zval);  // change rhs (theta' = theta + z)
            sub.optimize();  // optimize (compute dual multipliers)
            sub.subgradient(gamma,
                            kappa,
                            nu,
                            d_probs[scenario]);  // compute subgradient inequality
        }

        double rhs = gamma - nu * d_theta;

        for (size_t var = 0; var != d_n1; ++var)
            rhs += kappa[var] * d_x[var];

        stop = (etaval + nu * zval)
               >= (rhs
                   - tol);  // check if new cut is satisfied by current solution
        if (not stop)       // if not, then add cut
        {
            ++d_nCuts;  // increment cut counter
            d_constrs.push_back(
                d_model.addConstr(d_eta + nu * d_z, GRB_GREATER_EQUAL, rhs));

            // add the cut, and append it to internal storage
            d_gamma.push_back(gamma);
            d_kappa.push_back(std::vector<double>{kappa, kappa + d_n1});
            d_nu.push_back(nu);
        }
    }
}