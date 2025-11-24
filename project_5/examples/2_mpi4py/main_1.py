"""
HPC Class: Example of mpi4py
Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>

Description:
This program provides an example of using mpi4py for point-to-point communication between processes
in Python. It demonstrates message passing between multiple processes using MPI.

Installation Instructions:
Install mpi4py with the following command:
$ pip3.11 install mpi4py

Run Instructions:
Allocate the resources: salloc --nodes=4 --ntasks=8 --ntasks-per-node=2 --reservation=hpc-tuesday
To execute the program with MPI, use the following command:
$ mpirun -np 2 python3.11 main.py

TODO:
1) Comment on what the program is doing.
2) Run the program on 2 and 4 processes, observing the behavior in each case.
3) Discuss any issues that arise when running with only 1 process.
"""

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

data=np.zeros(10)

if rank == 0:
    comm.send(np.ones(10), dest=1, tag=100)
elif rank == 1:
    data=comm.recv(source=0, tag=100)

time.sleep(rank)
print(f"rank{rank} has {data}")