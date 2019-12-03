#include "zk.h"

void ZK::compute_tab_row_y(double *tab_row_y, int row)
{
  fill_n(tab_row_y, d_nVars, 0);

  int nTotalVars = d_nVars + d_nConstrs;
  int BW_row_ind[nTotalVars]; 
  double BW_row_val[nTotalVars]; 
  GRBsvec BW_row{nTotalVars, BW_row_ind, BW_row_val};
  GRBBinvRowi(d_model, row, &BW_row);
  
  for (size_t nz = 0; nz != BW_row.len; ++nz)
  {
    size_t var = BW_row.ind[nz];
    if (var < d_nVars)
      tab_row_y[var] = BW_row.val[nz];  
  }
}