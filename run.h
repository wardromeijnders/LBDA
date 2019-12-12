#ifndef RUN_H
#define RUN_H


#include "smips/problem_data/data.h"
#include "smips/problem_data/problem.h"


void run_ssv(Data &rand,
             GRBEnv &env,
             GRBenv *c_env,
             bool fs_cont,
             bool ss_bin,
             bool standard_T);

void run_sslp(Data &rand, GRBEnv &env, GRBenv *c_env);

void run_sizes(Data &rand, GRBEnv &env, GRBenv *c_env);

#endif  // RUN_H
