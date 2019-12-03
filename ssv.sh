#!/bin/bash
#SBATCH --time=10:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --job-name=ssv
#SBATCH --output=ssv_output/job-%j.log
#SBATCH --mem=16000
module load foss/2018a
module load Gurobi/7.0.2
./ssv $*