#!/bin/bash
#SBATCH --job-name=weak_multi
#SBATCH --output=std/weak_multi_%j.out
#SBATCH --error=std/weak_multi_%j.err
#SBATCH --nodes=16
#SBATCH --ntasks=16
#SBATCH --exclusive
#SBATCH --cpus-per-task=1
#SBATCH --time=00:20:00
#SBATCH --exclusive

module load gcc openmpi

make clean
make

DATAFILE="std/weak_multi.data"

echo "# p  n  time" > "$DATAFILE"

for p in $(seq 1 16); do
    n=$(python3 - <<EOF
import math
print(int(10000*math.sqrt($p)))
EOF
)
    echo "Running p=$p nodes n=$n"
    srun --nodes=$p --ntasks=$p --ntasks-per-node=1 \
        ./powermethod_rows 3 $n 300 -1e-6 >> "$DATAFILE"
done