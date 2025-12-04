from mandelbrot_task import *
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
from mpi4py import MPI
import numpy as np
import sys
import time

# some parameters
MANAGER = 0       # rank of manager
TAG_TASK      = 1 # task       message tag
TAG_TASK_DONE = 2 # tasks done message tag
TAG_DONE      = 3 # done       message tag

def manager(comm, tasks):
    """
    The manager.

    Parameters
    ----------
    comm : mpi4py.MPI communicator
        MPI communicator
    tasks : list of objects with a do_task() method perfroming the task
        List of tasks to accomplish

    Returns
    -------
    ... ToDo ...
    """
    size = comm.Get_size()
    num_workers = size - 1
    
    # Trackers
    tasks_done = []
    tasks_per_worker = np.zeros(size, dtype=int)
    
    # Task queue index
    task_index = 0
    num_tasks = len(tasks)
    
    # Count how many workers have been sent the shutdown signal
    closed_workers = 0

    print(f"Manager starting with {num_tasks} tasks and {num_workers} workers.")

    # ------------------------------------------------------
    # 1. Initial Distribution: Send one task to each worker
    # ------------------------------------------------------
    for worker_rank in range(1, size):
        if task_index < num_tasks:
            comm.send(tasks[task_index], dest=worker_rank, tag=TAG_TASK)
            task_index += 1
        else:
            # If we have more workers than tasks, shut down extra workers immediately
            comm.send(None, dest=worker_rank, tag=TAG_DONE)
            closed_workers += 1

    # ------------------------------------------------------
    # 2. Dynamic Scheduling Loop
    # ------------------------------------------------------
    while closed_workers < num_workers:
        # Wait for ANY worker to return a result
        status = MPI.Status()
        completed_task = comm.recv(source=MPI.ANY_SOURCE, tag=TAG_TASK_DONE, status=status)
        source_rank = status.Get_source()
        
        # Store result and update stats
        tasks_done.append(completed_task)
        tasks_per_worker[source_rank] += 1
        
        # Send next task to the SAME worker who just finished
        if task_index < num_tasks:
            comm.send(tasks[task_index], dest=source_rank, tag=TAG_TASK)
            task_index += 1
        else:
            # No more tasks: send termination signal
            comm.send(None, dest=source_rank, tag=TAG_DONE)
            closed_workers += 1

    return tasks_done, tasks_per_worker

def worker(comm):
    """
    The worker.

    Parameters
    ----------
    comm : mpi4py.MPI communicator
        MPI communicator
    """
    while True:
        status = MPI.Status()
        # Wait for any message from Manager
        task = comm.recv(source=MANAGER, tag=MPI.ANY_TAG, status=status)
        tag = status.Get_tag()
        
        if tag == TAG_DONE:
            # Shutdown signal received
            break
        elif tag == TAG_TASK:
            # Perform computation
            task.do_work()
            # Send result back to Manager
            comm.send(task, dest=MANAGER, tag=TAG_TASK_DONE)

def readcmdline(rank):
    """
    Read command line arguments

    Parameters
    ----------
    rank : int
        Rank of calling MPI process

    Returns
    -------
    nx : int
        number of gridpoints in x-direction
    ny : int
        number of gridpoints in y-direction
    ntasks : int
        number of tasks
    """
    # report usage
    if len(sys.argv) != 4:
        if rank == MANAGER:
            print("Usage: manager_worker nx ny ntasks")
            print("  nx     number of gridpoints in x-direction")
            print("  ny     number of gridpoints in y-direction")
            print("  ntasks number of tasks")
        sys.exit()

    nx = int(sys.argv[1])
    if nx < 1: 
        sys.exit("nx must be a positive integer")
    ny = int(sys.argv[2])
    if ny < 1: 
        sys.exit("ny must be a positive integer")
    ntasks = int(sys.argv[3])
    if ntasks < 1: 
        sys.exit("ntasks must be a positive integer")

    return nx, ny, ntasks


if __name__ == "__main__":

    # get COMMON WORLD communicator, size & rank
    comm    = MPI.COMM_WORLD
    size    = comm.Get_size()
    my_rank = comm.Get_rank()

    nx, ny, ntasks = readcmdline(my_rank)

    # ------------------------------------------------------
    # MANAGER LOGIC
    # ------------------------------------------------------
    if my_rank == MANAGER:
        print(f"MPI initialized with {size:5d} processes")
        
        # start timer
        timespent = - time.perf_counter()

        # trying out ... YOUR MANAGER-WORKER IMPLEMENTATION HERE ...
        x_min = -2.
        x_max  = +1.
        y_min  = -1.5
        y_max  = +1.5
        M = mandelbrot(x_min, x_max, nx, y_min, y_max, ny, ntasks)
        tasks = M.get_tasks()

        # RUN MANAGER
        tasks_done, TasksDoneByWorker = manager(comm, tasks)

        # Reconstruct image
        m = M.combine_tasks(tasks_done)
        plt.imshow(m.T, cmap="gray", extent=[x_min, x_max, y_min, y_max])
        plt.savefig("mandelbrot.png")

        # stop timer
        timespent += time.perf_counter()

        # inform that done
        print(f"Run took {timespent:f} seconds")
        for i in range(size):
            if i == MANAGER:
                continue
            print(f"Process {i:5d} has done {TasksDoneByWorker[i]:10d} tasks")
        print("Done.")

    # ------------------------------------------------------
    # WORKER LOGIC
    # ------------------------------------------------------
    else:
        worker(comm)