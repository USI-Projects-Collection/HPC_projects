#!/bin/bash
#SBATCH -A es_math                       # Account (es_math or ls_math)
#SBATCH --job-name=slurm_stream_c_1      # Job name    (default: sbatch)
#SBATCH --output=slurm_stream_c_1-%j.out # Output file (default: slurm-%j.out)
#SBATCH --error=slurm_stream_c_1-%j.err  # Error file  (default: slurm-%j.out)
#SBATCH --ntasks=1                       # Number of tasks
#SBATCH --cpus-per-task=1                # Number of CPUs per task
#SBATCH --time=00:01:00                  # Wall clock time limit

# load some modules & list loaded modules
module list
module load gcc

# print CPU info
lscpu | grep "Model name"

# compile & run
make clean
make
srun ./stream_c_1
