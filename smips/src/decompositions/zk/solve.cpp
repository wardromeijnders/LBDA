#include "decompositions/zk.h"

void ZK::solve(double *x,
               double theta,
               GRBmodel *master,
               std::vector<double> &kappa,
               std::vector<std::vector<double>> &beta,
               std::vector<double> &gamma,
               size_t maxRounds)
{
    double yvals[d_p2];  // to store values of integer decision vars

    bool stop = false;
    size_t round = 0;

    while (not stop)
    {
        GRBoptimize(d_model);  // solve the gurobimodel

        ++round;
        stop = true;  // resets to false if solution is non-integer

        if (round >= maxRounds)
            break;

        int bhead[d_nConstrs];
        GRBgetBasisHead(d_model, bhead);  // extract basis info
        GRBgetdblattrarray(d_model,
                           GRB_DBL_ATTR_X,
                           0,
                           d_p2,
                           yvals);  // extract values of integer variables

        size_t nCuts = 0;

        // loop over rows of simplex tableau
        for (size_t row = 0; row != d_nConstrs; ++row)
        {
            int basic_var = bhead[row];  // index of corresponding basic variable
            if (basic_var >= d_p2)       // check if variable has to be integer
                continue;                // if not, do not derive a cut

            double yval = yvals[basic_var];
            if (is_integer(yval))  // if variable value is integer,
                continue;          // then do not derive a cut

            stop = false;  // solution is not integer, derive cut

            int nVarsMaster;  // number of variables in master problem
                              // (including slacks)
            GRBgetintattr(master, "NumVars", &nVarsMaster);

            // computing tableau row
            double tab_row_x[nVarsMaster];
            compute_tab_row_x(
                tab_row_x,
                nVarsMaster,
                row,
                master);  // tableau row for (theta, x) (in that order)

            double tab_row_y[d_nVars];
            compute_tab_row_y(tab_row_y, row);  // tableau row for y variables

            double coef_x[nVarsMaster - 1];
            double coef_y[d_nVars];
            double coef_theta = -1;
            double coef_rhs = 1;  // cut coefficients

            double a0 = yval;

            gmi_cut(tab_row_x,
                    tab_row_y,
                    a0,
                    coef_x,
                    coef_y,
                    coef_theta,
                    nVarsMaster);

            transform_cut(coef_x,
                          coef_theta,
                          coef_rhs,
                          kappa,
                          beta,
                          gamma,
                          nVarsMaster - d_n1 - 1);

            add_cut(coef_x,
                    coef_y,
                    coef_theta,
                    coef_rhs,
                    x,
                    theta,
                    nVarsMaster - 1,
                    d_nConstrs + nCuts);


            ++nCuts;
        }

        d_nConstrs += nCuts;
        d_nVars += nCuts;  // slacks
    }
}