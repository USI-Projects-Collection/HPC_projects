#!/bin/bash
#SBATCH --job-name=pde_test
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=01:00:00
#SBATCH --output=pde_test_%j.out
#SBATCH --error=pde_test_%j.err

module load gcc

make clean
make

export OMP_NUM_THREADS=1

# Parametri
N=128          # griglia 128x128
NT=100         # 100 time steps
TF=0.005       # tempo finale

echo "Running PDE with parameters:
    N=${N}, NT=${NT}, TF=${TF}, threads=${OMP_NUM_THREADS}"

./main ${N} ${NT} ${TF}
