#!/bin/bash
#SBATCH --job-name=pde_strong
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --exclusive
#SBATCH --output=strong_%j.out
#SBATCH --error=strong_%j.err

module load gcc openmpi

make clean
make

NS=(64 128 256 512 1024)
PROCS=(1 2 4 8 16)
t_steps=100
dt=0.005


for N in "${NS[@]}"; do
    for P in "${PROCS[@]}"; do
        srun --ntasks=$P ./main $N $t_steps $dt
    done
done
