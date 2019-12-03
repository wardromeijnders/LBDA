#!/bin/bash
#SBATCH --time=00:02:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --job-name=cpptest
#SBATCH --mem=16000
module load foss/2018a
module load Gurobi/7.0.2
./exe $*