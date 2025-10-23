#!/bin/bash
#SBATCH --job-name=recur_scaling
#SBATCH --output=recur_scaling-%j.out
#SBATCH --error=recur_scaling-%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --mem-per-cpu=1024
#SBATCH --time=01:30:00
#SBATCH --exclusive

module load gcc

make clean
make

THREADS=(1 2 4 8 16 20)
mkdir -p logs

for t in "${THREADS[@]}"; do
  export OMP_NUM_THREADS="$t"
  echo "OMP_NUM_THREADS=${t}"
  ./recur_omp > "logs/recur_par_T${t}.log"
  if [[ "$t" == "1" ]]; then
    echo "OMP_NUM_THREADS=1"
    ./recur_seq > "logs/recur_seq_T1.log"
  fi
  echo "----------------------------------------"
  sleep 1
  make clean
  make
 done
