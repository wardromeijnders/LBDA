#include "ald.h"

Ald::Ald(const Ald &other)
:
  d_n1(other.d_n1), 
  d_n2(other.d_n2), 
  d_p2(other.d_p2), 
  d_m2(other.d_m2),
  d_Tmat(other.d_Tmat)
{
  GRBupdatemodel(other.d_model);
  d_model = GRBcopymodel(other.d_model);  
}