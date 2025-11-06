#!/bin/bash
#SBATCH --job-name=pde_weak_scaling
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH --time=04:00:00
#SBATCH --exclusive
#SBATCH --output=weak_scaling_%j.out
#SBATCH --error=weak_scaling_%j.err

module load gcc

make clean
make

NT=100
TF=0.005

BASE_RESOLUTIONS=(64 128 256)
THREADS=(1 2 4 8 16)

echo "Starting weak scaling tests" > weak_scaling_results.txt

for BASE in "${BASE_RESOLUTIONS[@]}"; do
  echo "------------------------------------------" | tee -a weak_scaling_results.txt
  echo "Base resolution: n=${BASE}" | tee -a weak_scaling_results.txt
  for T in "${THREADS[@]}"; do
    N=$(printf "%.0f" $(echo "${BASE}*sqrt(${T})" | bc -l))
    export OMP_NUM_THREADS=$T
    echo "==========================================" | tee -a weak_scaling_results.txt
    echo "Running with base=${BASE}, computed N=${N}, NT=${NT}, TF=${TF}, threads=${T}" | tee -a weak_scaling_results.txt
    ./main ${N} ${NT} ${TF} | tee -a weak_scaling_results.txt
  done
done
