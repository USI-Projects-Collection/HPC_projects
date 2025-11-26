#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=4

#SBATCH --time=00:10:00
#SBATCH --exclusive
#SBATCH --output=python_%j.out

module load python mpi4py gcc openmpi

srun python3 sum_ranks_buffer.py 
srun python3 sum_ranks_pickle.py