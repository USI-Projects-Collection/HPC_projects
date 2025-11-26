#SBATCH --nodes=2
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=4

module load python mpi4py gcc openmpi

srun python3 sum_ranks_buffer.py 
srun python3 sum_ranks_pickle.py