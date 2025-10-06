#!/bin/bash
#SBATCH -A es_math                       # Account (es_math or ls_math)
#SBATCH --job-name=slurm_hwloc-ls_1      # Job name    (default: sbatch)
#SBATCH --output=slurm_hwloc-ls_1-%j.out # Output file (default: slurm-%j.out)
#SBATCH --error=slurm_hwloc-ls_1-%j.err  # Error file  (default: slurm-%j.out)
#SBATCH --ntasks=1                       # Number of tasks
#SBATCH --cpus-per-task=1                # Number of CPUs per task
#SBATCH --time=00:01:00                  # Wall clock time limit

# load some modules & list loaded modules
module load hwloc
module list

# print CPU info
lscpu

# print cache hierarchy
hwloc-ls --whole-system --no-io
hwloc-ls --whole-system --no-io -f --of fig INTEL_XEON_E5-2650.fig
