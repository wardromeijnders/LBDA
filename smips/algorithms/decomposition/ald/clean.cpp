#include "ald.h"

void Ald::clean()
{

  int nConstrs;

  GRBupdatemodel(d_model); 

  GRBgetintattr(d_model, "NumConstrs", &nConstrs);
  int nCuts = nConstrs - d_m2; 
  
  
  int cutInds[nCuts];
  int slackInds[nCuts];
  iota(cutInds, cutInds + nCuts, d_m2); 
  iota(slackInds, slackInds + nCuts, d_n2);
  
  GRBdelconstrs(d_model, nCuts, cutInds);
  GRBdelvars(d_model, nCuts, slackInds);
}