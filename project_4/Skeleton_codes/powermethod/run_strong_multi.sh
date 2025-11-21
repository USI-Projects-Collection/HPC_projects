#!/bin/bash
#SBATCH --job-name=strong_multi
#SBATCH --output=std/strong_multi_%j.out
#SBATCH --error=std/strong_multi_%j.err
#SBATCH --nodes=16
#SBATCH --ntasks=16
#SBATCH --exclusive 
#SBATCH --cpus-per-task=1
#SBATCH --time=00:20:00
#SBATCH --exclusive

module load gcc openmpi

make clean
make

DATAFILE="std/strong_multi.data"

echo "# p  n  time" > "$DATAFILE"

for p in $(seq 1 16); do
    echo "Running with p = $p nodes"
    srun --nodes=$p --ntasks=$p --ntasks-per-node=1 \
        ./powermethod_rows 3 10000 300 -1e-6 >> "$DATAFILE"
done