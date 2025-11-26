#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=4

#SBATCH --time=00:10:00
#SBATCH --exclusive
#SBATCH --output=python_%j.out
#SBATCH --error=python_%j.err

module load python mpi4py gcc openmpi

mpiexec python3 sum_ranks_buffer.py 
mpiexec python3 sum_ranks_pickle.py