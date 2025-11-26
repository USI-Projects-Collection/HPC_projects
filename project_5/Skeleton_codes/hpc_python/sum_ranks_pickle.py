from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

local_value = rank

total = comm.allreduce(local_value, op=MPI.SUM)

if rank == 0:
    print(f"[pickle] Sum of ranks over {size} processes = {total}")