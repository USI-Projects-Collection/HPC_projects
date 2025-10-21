#!/bin/bash
#SBATCH --job-name=dotproduct
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --time=02:00:00
#SBATCH --output=dotprod_sweep_%j.out
#SBATCH --error=dotprod_sweep_%j.err
#SBATCH --exclusive

module load gcc

# Use seq to list the Ns we want to test (powers of 10).
SIZES=(100000 1000000 10000000 100000000)
THREADS=(1 2 4 8 16 20)

LOG_DIR="$(pwd)/logs"
mkdir -p "${LOG_DIR}"

for N in "${SIZES[@]}"; do
	make clean
	make N="${N}"

	for t in "${THREADS[@]}"; do
		export OMP_NUM_THREADS="${t}"
		LOG_FILE="${LOG_DIR}/dotprod_N${N}_T${t}.log"
		echo "Running N=${N}, OMP_NUM_THREADS=${t}"
		./dotProduct | tee "${LOG_FILE}"
	done
done
