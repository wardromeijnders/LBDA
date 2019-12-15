#include "decompositions/zk.h"

void ZK::compute_tab_row_x(double *tab_row_x,
                           int nVarsMaster,
                           int row,
                           GRBmodel *master)
{
    std::fill_n(tab_row_x, nVarsMaster, 0);
    // extracting i-th row of B^-1 (basis matrix of subproblem
    int e_i_ind[1] = {row};
    double e_i_val[1] = {1.0};
    GRBsvec e_i{1, e_i_ind, e_i_val};  // unit vector

    int Brow_ind[d_nConstrs];
    double Brow_val[d_nConstrs];
    GRBsvec Brow{static_cast<int>(d_nConstrs),
                 Brow_ind,
                 Brow_val};  // result vector

    GRBBSolve(d_model, &e_i, &Brow);  // extracting ith row of B^-1

    // extracting master basis information
    int nConsMaster;  // number of constraints in master problem (including cuts)
    GRBgetintattr(master, "NumConstrs", &nConsMaster);

    int master_bhead[nConsMaster];
    GRBgetBasisHead(master, master_bhead);

    // computing (BW^-1)_i * T_{BA}
    double BinvTBA[nConsMaster];
    std::fill_n(BinvTBA, nConsMaster, 0);

    for (size_t idx = 0; idx != nConsMaster; ++idx)
    {
        int basic_var = master_bhead[idx];  // column index of (tau, Tmat)

        if (basic_var == 0)  // then multiply by tau (column 0 of (tau, Tmat))
        {
            for (size_t nz = 0; nz != Brow.len;
                 ++nz)  // loop over nonzeros of Brow
            {
                size_t pos = Brow.ind[nz];  // position of nonzero element
                BinvTBA[idx] += Brow.val[nz] * d_tau[pos];
            }
        }

        --basic_var;  // column index of Tmat (-1 because T = (tau, Tmat))

        for (size_t nz = 0; nz != Brow.len; ++nz)  // non-zeroes of Brow
        {
            // position of nonzero element and corresponding row of T
            size_t pos = Brow.ind[nz];
            std::vector<double> &T_row = d_Tmat[pos];

            // check if element exists, otherwise it equals zero
            if (basic_var < T_row.size())
                BinvTBA[idx] += Brow.val[nz] * T_row[basic_var];
        }
    }

    for (size_t col = 0; col != nVarsMaster; ++col)
    {
        // extract column of master simplex tableau
        int BA_col_ind[nConsMaster];
        double BA_col_val[nConsMaster];
        GRBsvec BA_col{nConsMaster, BA_col_ind, BA_col_val};
        GRBBinvColj(master, col, &BA_col);

        for (size_t idx = 0; idx != BA_col.len; ++idx)
        {
            size_t row = BA_col.ind[idx];
            tab_row_x[col] -= BinvTBA[row] * BA_col.val[idx];
        }
    }

    // TODO: SEGFAULTING?
    // add (B^W)^-1_i *tau to tab_row_x[0]
    for (size_t nz = 0; nz != Brow.len; ++nz)
    {
        size_t pos = Brow.ind[nz];
        tab_row_x[0] += Brow.val[nz] * d_tau[pos];
    }

    for (size_t idx = 1; idx != nVarsMaster; ++idx)  // computing tab_row_x[idx]
    {
        size_t col = idx - 1;  // column index of Tmat
        for (size_t nz = 0; nz != Brow.len; ++nz)
        {
            size_t pos = Brow.ind[nz];
            std::vector<double> &Trow = d_Tmat[pos];

            // check if Trow[col] exists, otherwise it equals zero
            if (col < Trow.size())
                tab_row_x[idx] += Trow[col] * Brow.val[nz];
        }
    }
}