#!/bin/bash
#SBATCH --job-name=pde_strong_scaling
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH --time=04:00:00
#SBATCH --exclusive
#SBATCH --output=strong_scaling_%j.out
#SBATCH --error=strong_scaling_%j.err

module load gcc

make clean
make

NT=100
TF=0.005

GRID_SIZES=(64 128 256 512 1024)
THREADS=(1 2 4 8 16)

echo "Starting strong scaling tests" > strong_scaling_results.txt

for N in "${GRID_SIZES[@]}"; do
  for T in "${THREADS[@]}"; do
    export OMP_NUM_THREADS=$T
    echo "==========================================" | tee -a strong_scaling_results.txt
    echo "Running with N=${N}, NT=${NT}, TF=${TF}, threads=${T}" | tee -a strong_scaling_results.txt
    ./main ${N} ${NT} ${TF} | tee -a strong_scaling_results.txt
  done
done
