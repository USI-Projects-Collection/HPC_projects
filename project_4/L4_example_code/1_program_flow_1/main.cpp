/*
 * HPC Assignment: MPI Basics
 * Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>
 *
 * Description:
 * This program provides a basic flow of an MPI program, including initializing the MPI environment,
 * distributing tasks across processes, and gathering results.
 *
 * Compilation Instructions:
 * To compile the program (see make file) with both OpenMP and MPI support, use the following command:
 * mpic++ -fopenmp main.cpp -o main.exe
 *
 * Run Instructions:
 * To run the program across multiple processes, use mpirun with the desired number of processes:
 * $ mpirun -np 2 ./main.exe
 *
 * TODO:
 * 1) Examine and comment on the operations being performed by each process in the program.
 *    Observe the output for each process when running with two processes using:
 *    $ mpirun -np 2 ./main.exe
 * 2) Compare this MPI-based code to a sequential version using a standard loop, such as:
 *    for (size_t i = 0; i < val.size(); i++){
 *        val[i] = some_func(i);
 *    }
 */

#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <vector>

double some_func(int i) {
  sleep(1);
  return -1;
}

int main() {

  int size, rank;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = size;
  std::vector<double> val(n, 0);

  printf("(Rank %d ) Before evaluation.\n", rank);
  for (int i = 0; i < val.size(); i++) {
    printf("%f ", val[i]);
  }
  printf("\n\n");

  val[rank] = some_func(rank);
  
  printf("(Rank %d ) After evaluation.\n", rank);
  for (int i = 0;i < val.size(); i++) {
    printf("%f ", val[i]);
  }
  printf("\n\n");

  MPI_Allreduce(MPI_IN_PLACE, &val[0], n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  printf("(Rank %d ) After MPI_Allreduce.\n", rank);

  for (int i = 0; i < val.size(); i++) {
    printf("%f ", val[i]);
  }
  printf("\n\n");

  MPI_Finalize();
  return 0;
}
