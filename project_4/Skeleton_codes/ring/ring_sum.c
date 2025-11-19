#include <mpi.h> // MPI
#include <stdio.h>

int main(int argc, char *argv[]) {

  // Initialize MPI, get size and rank
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // IMPLEMENT: Ring sum algorithm
  int next = (rank + 1) % size;
  int prev = (rank - 1 + size) % size;

  int send_val = rank;
  int sum = rank;

  for (int step = 0; step < size - 1; ++step) {
    int recv_val = 0;
    MPI_Sendrecv(
      &send_val, 1, MPI_INT, next, 0,
      &recv_val, 1, MPI_INT, prev, 0, 
      MPI_COMM_WORLD, MPI_STATUS_IGNORE
    );
    
    sum += recv_val;
    send_val = recv_val;
  }  
  
  printf("Process %i: Sum = %i\n", rank, sum);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
