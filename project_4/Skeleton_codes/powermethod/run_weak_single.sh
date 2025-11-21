#!/bin/bash
#SBATCH --job-name=weak_single
#SBATCH --output=std/weak_single_%j.out
#SBATCH --error=std/weak_single_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=20
#SBATCH --exclusive 
#SBATCH --cpus-per-task=1
#SBATCH --time=00:20:00
#SBATCH --exclusive

module load gcc openmpi

make clean
make

DATAFILE="std/weak_single.data"

echo "# p  n  time" > "$DATAFILE"

for p in $(seq 1 20); do
    n=$(python3 - <<EOF
import math
print(int(10000*math.sqrt($p)))
EOF
)
    echo "Running p=$p n=$n"
    srun --ntasks=$p ./powermethod_rows 3 $n 300 -1e-6 >> "$DATAFILE"
done