#!/bin/bash
#SBATCH --job-name=pde_weak
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=1
#SBATCH --time=02:00:00
#SBATCH --exclusive
#SBATCH --output=weak_%j.out
#SBATCH --error=weak_%j.err

module load gcc openmpi

make clean
make

BASE_N=64
PROCS=(1 2 4 8 16)
t_steps=100
dt=0.005

for P in "${PROCS[@]}"; do
    N=$(python3 - << EOF
import math
print(int(${BASE_N} * math.sqrt(${P})))
EOF
)
    srun --ntasks=$P ./main $N $t_steps $dt
done
