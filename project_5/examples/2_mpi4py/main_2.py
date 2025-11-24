"""
HPC Class: Example of mpi4py
Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>

Description:
This program provides an example of using mpi4py for collective communication between processes
in Python. It demonstrates row-block matrix multiplication using MPI.

Installation Instructions:
Install mpi4py with the following command:
$ pip3 install mpi4py

Run Instructions:
Allocate the resources: salloc --nodes=4 --ntasks=8 --ntasks-per-node=2 --reservation=hpc-tuesday
To execute the program with MPI, use the following command (dont forget to load openmpi):
$ mpirun -np 2 python3.11 main.py

TODO:
1) Comment on what each part of the program is doing.
2) Run the program on 2, 4, and 8 processes, observing the behavior in each case.
3) Discuss the implications of the observed behavior with increasing process counts.
"""

from mpi4py import MPI
import numpy as np
import time

np.set_printoptions(linewidth=200)  # Set width to 200 characters

# Initialize MPI
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
proc = MPI.Get_processor_name()

#np.random.seed(42)

# Define matrix dimensions
p = 1000
n = 100000
Y = np.random.normal(0, 1, (p, n))  # Full matrix Y with Gaussian random numbers

time_prl = -time.time()

S_full = np.zeros((p, p)) 

# Define the block rows each process will handle
block_size = p // size
start_row = rank * block_size
end_row = start_row + block_size

if rank == size-1:
    end_row = p

inx_range = range(start_row, end_row)

# Select the submatrix for this rank's computation
sub_Y = Y[inx_range, :]
Y_T = Y.T

# Perform row-block-wise matrix multiplication
S_sub = sub_Y @ Y_T/n
S_full[inx_range, :] = S_sub  # Store partial results in the full matrix

comm.Allreduce(MPI.IN_PLACE, S_full, op=MPI.SUM)

time_prl = time_prl + time.time()

if rank==0:

    time_seq = -time.time()
    S_rank1 = Y @ Y.T/n
    time_seq = time_seq+time.time()

    err = np.linalg.norm(S_full - S_rank1 )

    print(f"Speedup {time_seq/time_prl} Error = {err} ")