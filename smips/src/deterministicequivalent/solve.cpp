#include "deterministicequivalent.h"


std::unique_ptr<arma::vec> DeterministicEquivalent::solve(double timeLimit)
{
    d_model.set(GRB_DoubleParam_TimeLimit, timeLimit);
    d_model.optimize();

    int status = d_model.get(GRB_IntAttr_Status);

    if (status == GRB_INFEASIBLE)
    {
        d_status = status::INFEASIBLE;
        return std::make_unique<arma::vec>(nullptr);
    }

    d_status = status::SOLVED;

    auto const &Amat = d_problem.Amat();
    auto const *xPtr = d_model.get(GRB_DoubleAttr_X, d_xVars, Amat.n_rows);

    auto result = std::make_unique<arma::vec>(xPtr, Amat.n_rows);
    delete[] xPtr;

    return result;
}
