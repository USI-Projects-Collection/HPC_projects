#!/bin/bash
#SBATCH --job-name=pde_weak_scaling
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=1
#SBATCH --time=04:00:00
#SBATCH --exclusive
#SBATCH --output=weak_scaling_%j.out
#SBATCH --error=weak_scaling_%j.err

module load gcc openmpi

make clean
make

NT=100
TF=0.005

BASE_RESOLUTIONS=(64 128 256 512 1024)
PROCS=(1 2 4 8 16)

echo "Starting weak scaling tests" > weak_scaling_results.txt

for BASE in "${BASE_RESOLUTIONS[@]}"; do
  echo "------------------------------------------" | tee -a weak_scaling_results.txt
  echo "Base resolution: n=${BASE}" | tee -a weak_scaling_results.txt
  for T in "${PROCS[@]}"; do
    N=$(printf "%.0f" $(echo "${BASE}*sqrt(${T})" | bc -l))
    echo "==========================================" | tee -a weak_scaling_results.txt
    echo "Running with base=${BASE}, computed N=${N}, NT=${NT}, TF=${TF}, threads=${T}" | tee -a weak_scaling_results.txt
    srun --ntasks=$T ./main ${N} ${NT} ${TF} | tee -a weak_scaling_results.txt
  done
done
