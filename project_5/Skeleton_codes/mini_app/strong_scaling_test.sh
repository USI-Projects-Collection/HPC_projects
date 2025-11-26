#!/bin/bash
#SBATCH --job-name=pde_strong
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --output=strong_%j.out
#SBATCH --error=strong_%j.err

module load gcc openmpi

make clean
make

N=1024         
NT=100
TF=0.005
PROCS=(1 2 4 8 16)

for P in "${PROCS[@]}"; do
    srun --ntasks=$P ./main $N $NT $TF | tee -a strong_results.txt
done