#!/bin/bash
#SBATCH --job-name=mandel
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --time=01:00:00
#SBATCH --output=mandel_%j.out
#SBATCH --error=mandel_%j.err
#SBATCH --exclusive

module load gcc

THREADS=(1 2 4 8 16 20)
IMAGE_SIZES=(
  "1024x1024"
  "2048x2048"
  "4096x4096"
  "8192x8192"
)

LOG_DIR=$(pwd)/logs
mkdir -p "${LOG_DIR}"

for size in "${IMAGE_SIZES[@]}"; do
  WIDTH=${size%x*}
  HEIGHT=${size#*x}
  echo "Building for ${WIDTH}x${HEIGHT}"
  make clean
  make IMAGE_WIDTH=${WIDTH} IMAGE_HEIGHT=${HEIGHT}

  for t in "${THREADS[@]}"; do
    export OMP_NUM_THREADS="${t}"
    LOG_FILE="${LOG_DIR}/mandel_${size}_T${t}.log"
    IMAGE_FILE="mandel_${size}_T${t}.png"
    echo "Running size=${size}, threads=${t}" | tee "${LOG_FILE}"
    ./mandel_seq "${IMAGE_FILE}" >> "${LOG_FILE}"
  done
done
