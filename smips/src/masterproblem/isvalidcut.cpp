#include "masterproblem.h"


bool MasterProblem::isValidCut(Cut::CutResult const &cutResult,
                               MasterProblem::Solution const &sol,
                               double tol)
{
    return cutResult.gamma + arma::dot(*sol.x, cutResult.beta)
           >= sol.theta + tol;
}
