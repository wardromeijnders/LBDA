#include "decompositions/ald.h"

void Ald::clean()
{
    GRBupdatemodel(d_model);

    int nConstrs;
    GRBgetintattr(d_model, "NumConstrs", &nConstrs);

    int nCuts = nConstrs - d_m2;
    int cutInds[nCuts];
    int slackInds[nCuts];

    std::iota(cutInds, cutInds + nCuts, d_m2);
    std::iota(slackInds, slackInds + nCuts, d_n2);

    GRBdelconstrs(d_model, nCuts, cutInds);
    GRBdelvars(d_model, nCuts, slackInds);
}
