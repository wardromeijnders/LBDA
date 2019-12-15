#include "decompositions/zk.h"

ZK::ZK(ZK &&other) :
    d_n1(other.d_n1),
    d_p1(other.d_p1),
    d_n2(other.d_n2),
    d_p2(other.d_p2),
    d_nVars(other.d_nVars),
    d_nConstrs(other.d_nConstrs),
    d_omega(other.d_omega),
    d_Tmat(other.d_Tmat),
    d_tau(other.d_tau),
    d_model(other.d_model)
{
    other.d_model = nullptr;
}
