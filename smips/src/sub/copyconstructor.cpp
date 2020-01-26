#include "sub.h"

Sub::Sub(Sub const &other) :
    d_m2(other.d_m2),
    d_n2(other.d_n2),
    d_q(other.d_q),
    d_model(other.d_model)
{
    d_vars = d_model.getVars();
    d_constrs = d_model.getConstrs();
}
