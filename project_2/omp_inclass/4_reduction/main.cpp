/*
 HPC Class: OpenMP Critical, Atomic, and Reduction
 Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

 Description:
 This program demonstrates how to use OpenMP to parallelize a loop and handle 
 shared variables using three different techniques: critical sections, atomic 
 operations, and the reduction clause. 

 Allocation:
 srun -N 1 --exclusive  --time=00:05:00 --reservation=hpc-monday --pty bash -i

 Compilation Instructions:
 To compile the program with OpenMP support, use the following command:
 g++ -fopenmp -o main.exe main.cpp

 Run Instructions:
 Set the environment variable OMP_NUM_THREADS to control the number of threads:
 $ export OMP_NUM_THREADS=1; ./main.exe

 TODO:
 1) Execute the program with OMP_NUM_THREADS set to 1, 2, and 4.
    $ export OMP_NUM_THREADS=2; ./main.exe
 2) Comment on the observed behavior, particularly how the performance and 
    results differ between critical, atomic, and reduction examples.
*/

#include <omp.h>
#include <iostream>


int main() {

  int n = 1e6;
  int max_iter = 100;

  double *a = new double[n];
  double *b = new double[n];

  // Initialize the vectors with some values
  for (int i = 0; i < n; i++) {
    a[i] = double(i+1);
    b[i] = 1.0 / double(i+1);
  }

  long double res;
  double time_start;

  // serial execution
  time_start = omp_get_wtime();
  for (int itr = 0; itr < max_iter; itr++) {
    res = 0.0;
    for (int i = 0; i < n; i++) {
      res += a[i] * b[i];
    }
  }
  std::cout << "Serial:        time=" << omp_get_wtime() - time_start << " sec" << ", res="<<res << std::endl;

  // critical
  time_start = omp_get_wtime();
  for (int itr = 0; itr < max_iter; itr++) {
    res = 0.0;
    #pragma omp parallel for default(none) shared(a,b,n,res)
    for (int i = 0; i < n; i++) {
      #pragma omp critical
      res += a[i] * b[i];
    }
  }
  std::cout << "OMP Critical:  time=" << omp_get_wtime() - time_start << " sec" << ", res="<<res << std::endl;

  // atomic
  time_start = omp_get_wtime();
  for (int itr = 0; itr < max_iter; itr++) {
    res = 0.0;
    #pragma omp parallel for default(none) shared(a,b,n,res)
    for (int i = 0; i < n; i++) {
      #pragma omp atomic update
      res += a[i] * b[i];
    }
  }
  std::cout << "OMP Atomic:    time=" << omp_get_wtime() - time_start << " sec" << ", res="<<res << std::endl;

  // reduction
  time_start = omp_get_wtime();
  for (int itr = 0; itr < max_iter; itr++) {
    res = 0.0;
    #pragma omp parallel for default(none) shared(a,b,n) reduction(+ : res)
    for (int i = 0; i < n; i++) {
      res += a[i] * b[i];
    }
  }
  std::cout << "OMP Reduction: time=" << omp_get_wtime() - time_start << " sec" << ", res="<<res << std::endl;

  // De-allocate memory
  delete[] a;
  delete[] b;

  return 0;
}
