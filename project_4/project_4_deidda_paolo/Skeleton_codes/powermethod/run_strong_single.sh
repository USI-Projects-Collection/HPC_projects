#!/bin/bash
#SBATCH --job-name=strong_single
#SBATCH --output=std/strong_single_%j.out
#SBATCH --error=std/strong_single_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1
#SBATCH --time=00:20:00
#SBATCH --exclusive

module load gcc openmpi

make clean
make

DATAFILE="std/strong_single.data"

echo "# p  n  time" > "$DATAFILE"

for p in $(seq 1 20); do
    echo "Running with p = $p"
    srun --ntasks=$p ./powermethod_rows 3 10000 300 -1e-6 >> "$DATAFILE"
done