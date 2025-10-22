#!/bin/bash
#SBATCH --job-name=pi_midpoint
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:20:00
#SBATCH --partition=debug
#SBATCH --output=pi_midpoint_%j.out
#SBATCH --error=pi_midpoint_%j.err

module load gcc

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

LOG_DIR="logs"
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
