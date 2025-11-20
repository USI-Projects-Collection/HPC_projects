#!/bin/bash
#SBATCH --job-name=ring
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --time=00:10:00
#SBATCH --exclusive
#SBATCH --output=ring_%j.out
#SBATCH --error=ring_%j.err

module load openmpi

make clean
make

srun ./ring_sum
