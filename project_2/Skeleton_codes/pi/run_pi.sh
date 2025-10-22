#!/bin/bash
#SBATCH --job-name=pi_midpoint
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=01:00:00
#SBATCH --output=pi_midpoint_%j.out
#SBATCH --error=pi_midpoint_%j.err

module load gcc

LOG_DIR=$(pwd)/logs
mkdir -p "${LOG_DIR}"

make clean
make

THREADS=(1 2 4 8)

for t in "${THREADS[@]}"; do
    export OMP_NUM_THREADS="${t}"
    LOG_FILE="${LOG_DIR}/pi_T${t}.log"
    echo "Running with OMP_NUM_THREADS=${t}"
    ./pi > "${LOG_FILE}"
done

echo "Runs completed. Logs stored in ${LOG_DIR}"
