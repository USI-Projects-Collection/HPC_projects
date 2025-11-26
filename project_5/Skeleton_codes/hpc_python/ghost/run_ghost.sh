#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=4

#SBATCH --time=00:10:00
#SBATCH --exclusive
#SBATCH --output=python_%j.out
#SBATCH --error=python_%j.err

# conda
source /apps/miniconda3/bin/activate
conda activate project5_env

module load gcc openmpi

mpiexec python3 ghost.py
