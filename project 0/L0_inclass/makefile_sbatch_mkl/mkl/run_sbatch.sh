#!/bin/bash
#SBATCH --job-name=TEST_MKL           # Job name  
#SBATCH --output=X_out_%j.out         # Output file (default: slurm-%j.out)
#SBATCH --error=X_err_%j.err          # Error file  (default: slurm-%j.err)
#SBATCH --partition=slim              # Partition
#SBATCH --nodes=1                     # Number of nodes to allocate for the job.
#SBATCH --ntasks-per-node=1           # Number of tasks (MPI processes) per node.
#SBATCH --ntasks-per-core=1           # Keep this 1 (hyperthreading)
#SBATCH --cpus-per-task=1             # Number of CPUs per task
#SBATCH --time=00:01:00               # Wall clock time limit

# load some modules & list loaded modules
module load intel-oneapi-mkl
module load gcc
module list

./main.exe