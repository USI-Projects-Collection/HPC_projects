#!/bin/bash
#SBATCH --job-name=ghost
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --time=00:20:00
#SBATCH --exclusive
#SBATCH --output=ghost_%j.out
#SBATCH --error=ghost_%j.err

module load openmpi

make clean
make

srun ./ghost
