#include "decompositions/strongbenders.h"


void StrongBenders::update(arma::vec &rhs, arma::vec &pi)
{
    auto const &Amat = d_problem.Amat();
    d_model.set(GRB_DoubleAttr_Obj, d_z_vars, pi.memptr(), Amat.n_rows);

    auto const &Wmat = d_problem.Wmat();
    d_model.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), Wmat.n_cols);
}
