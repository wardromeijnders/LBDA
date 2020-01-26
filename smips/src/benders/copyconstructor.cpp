#include "benders.h"

Benders::Benders(const Benders &other) :
    d_n1(other.d_n1),
    d_n2(other.d_n2),
    d_m2(other.d_m2),
    d_S(other.d_S),
    d_nCuts(other.d_nCuts),
    d_runTime(other.d_runTime),
    d_env(other.d_env),
    d_problem(other.d_problem),
    d_master(other.d_master),
    d_sub(other.d_sub),
    d_lr(other.d_lr),
    d_gomory(other.d_gomory),
    d_visited(other.d_visited),
    d_objectives(other.d_objectives)
{
    d_xvals = new double[d_n1];
}
