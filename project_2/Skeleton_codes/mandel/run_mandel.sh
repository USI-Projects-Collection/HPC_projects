#!/bin/bash
#SBATCH --job-name=mandel
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:30:00
#SBATCH --output=mandel_%j.out
#SBATCH --error=mandel_%j.err
#SBATCH --exclusive

module load gcc

make clean
make mandel_seq

THREADS=(1 2 4 8)
LOG_DIR=$(pwd)/logs
mkdir -p "${LOG_DIR}"

for t in "${THREADS[@]}"; do
  export OMP_NUM_THREADS="${t}"
  echo "Running with OMP_NUM_THREADS=${t}" | tee "${LOG_DIR}/mandel_T${t}.log"
  ./mandel_seq >> "${LOG_DIR}/mandel_T${t}.log"
done
