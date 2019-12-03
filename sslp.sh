#!/bin/bash
#SBATCH --time=00:20:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --job-name=sslp
#SBATCH --output=sslp_output/job-%j.log
#SBATCH --mem=16000
module load foss/2018a
module load Gurobi/7.0.2
./sslp $*