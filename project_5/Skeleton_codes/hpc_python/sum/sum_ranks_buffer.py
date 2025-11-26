from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

buf_local = np.array(rank, dtype='i')
buf_global = np.zeros(1, dtype='i')

comm.Allreduce(buf_local, buf_global, op=MPI.SUM)

if rank == 0:
    print(f"[buffer] Sum of ranks over {size} processes = {buf_global[0]}")