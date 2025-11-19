#!/bin/bash
#SBATCH --job-name=TEST_OMP_MPI       # Job name    (default: sbatch)
#SBATCH --output=log.out              # Output file (default: slurm-%j.out)
#SBATCH --error=log.err               # Error file  (default: slurm-%j.err)
#SBATCH --partition=slim              # Partition
#SBATCH --nodes=4                     # Number of nodes to allocate for the job.
#SBATCH --ntasks-per-node=1           # Number of tasks (MPI processes) per node
#SBATCH --ntasks-per-core=1           # Keep this 1  (hyperthreading)
#SBATCH --cpus-per-task=20            # Number of CPUs per task
#SBATCH --time=00:01:00               # Wall clock time limit

# load some modules & list loaded modules
module load openmpi/5.0.5
module load gcc
module list

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun ./main.exe