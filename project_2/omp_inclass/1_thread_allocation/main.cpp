/*
 HPC Class : OpenMP Thread Count Allocation
 Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>

 Description:
 This program demonstrates the basic usage of OpenMP with a focus on 
 allocating and controlling the number of threads. 

 Allocation:
 srun -N 1 --exclusive  --time=00:05:00 --reservation=hpc-monday --pty bash -i

 Compilation Instructions:
 To compile the program with OpenMP support, use the following command:
 g++ -fopenmp -o main.exe main.cpp

 Run Instructions:
 Set the environment variable OMP_NUM_THREADS before executing the program:
 $ export OMP_NUM_THREADS=1; ./main.exe

 TODO:
 1) Execute the program by setting OMP_NUM_THREADS to 4:
    $ export OMP_NUM_THREADS=4; ./main.exe
 2) Experiment with different values for OMP_NUM_THREADS (e.g., 2, 6, etc.).
 3) Comment on how varying the number of threads affects the output and 
    the behavior of the program in each parallel region.
*/

#include <omp.h>
#include <iostream>

int main() {

  #pragma omp parallel
  {
    printf("parallel region 1: We are %d threads, I am thread %d.\n", omp_get_num_threads(), omp_get_thread_num());
    
  }
  printf("\n");

  omp_set_num_threads(2);

  #pragma omp parallel num_threads(3)
  {
    printf("parallel region 2: We are %d threads, I am thread %d.\n", omp_get_num_threads(), omp_get_thread_num());
  }
  printf("\n");

  #pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    printf("parallel region 3: We are %d threads, I am thread %d.\n", omp_get_num_threads(), omp_get_thread_num());
  }
  printf("\n");

  return 0;
}

