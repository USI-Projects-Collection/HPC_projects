/*
 HPC Class: OpenMP Race Conditions
 Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>

 Description:
 This program demonstrates a common issue in parallel computing known as 
 a race condition. It uses OpenMP to parallelize a simple loop where a shared 
 variable `sum` is updated by multiple threads simultaneously, leading to 
 incorrect results if not handled properly. A race condition occurs when 
 multiple threads attempt to modify the same variable concurrently without 
 synchronization.

 Allocation:
 srun -N 1 --exclusive  --time=00:05:00 --reservation=hpc-monday --pty bash -i

 Compilation Instructions:
 To compile the program with OpenMP support, use the following command:
 g++ -fopenmp -o main.exe main.cpp

 Run Instructions:
 Set the environment variable OMP_NUM_THREADS before executing the program:
 $ export OMP_NUM_THREADS=1; ./main.exe

 TODO:
 1) Execute the program by setting OMP_NUM_THREADS to 1 and then >1 
    (e.g., 2 or 4):
    $ export OMP_NUM_THREADS=2; ./main.exe
 2) Comment on what you expect to happen and what you actually observe.
 3) Use the `reduction(+:sum)` clause to prevent a race condition. 
*/

#include <omp.h>
#include <iostream>

int main() {

  double sum=0.0;
  double b=5.0;

  int N = 1e5;
  
  // To fix the race condition, replace 'shared(sum)' with 'reduction(+:sum)'
  // There are other ways!
  #pragma omp parallel for default(none) shared(b,sum,N)
  for (int i = 0; i < N; i++){
    sum+= 1.0;
  }
  
  if(int(sum)!=N){
    std::cout<< "Something is wrong!" <<std::endl;
  }

  std::cout<< sum <<std::endl;

  return 0;
}