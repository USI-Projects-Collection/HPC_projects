/*
 HPC Class: OpenMP Scheduling (Static vs. Dynamic)
 Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

 Description:
 This program demonstrates the basics of OpenMP scheduling, comparing static 
 and dynamic scheduling methods.

 Compilation Instructions:
 To compile the program with OpenMP support, use the following command:
 g++ -fopenmp -o main.exe main.cpp

 Allocation:
 srun -N 1 --exclusive  --time=00:05:00 --reservation=hpc-monday --pty bash -i

 Run Instructions:
 Set the number of threads and run the program:
 $ export OMP_NUM_THREADS=4; ./main.exe

 TODO:
 1) Run the program with OMP_NUM_THREADS set to 4 and measure the execution time.
    $ export OMP_NUM_THREADS=4; time ./main.exe
 2) Experiment with both regular and irregular workloads.
 3) Experiment with schedule(static), schedule(dynamic), and schedule(static,1) scheduling and compare their performance.
 4) Note the schedule(dynamic) and schedule(static,1) cases — analyze how they affect memory access performance.
*/


#include <omp.h>
#include <iostream>
#include <vector>
#include <unistd.h>

void regular_workload(int i){
  sleep(1);
}

void irregular_workload(int i){
  if(i>3){
    sleep(1);
  }
}

int main() {

  int N = 8;
  int T = omp_get_max_threads();
  std::vector<double> time_reg(T,0.0);

  double t_total = omp_get_wtime();

  #pragma omp parallel for default(none) shared(time_reg,N) schedule(static)
  for (int i = 0; i < N; i++){
    int tid = omp_get_thread_num();
    double t = omp_get_wtime();

    regular_workload(i);
    //irregular_workload(i);

    time_reg[tid]+= omp_get_wtime()  - t;

    printf("itr %d -> tid %d \n",i,tid);
  }

  t_total = omp_get_wtime()-t_total;
    
  printf("Time = ");
  for (int i = 0; i < T; i++){
    printf("%f ",time_reg[i]);
  }
  printf("\n");
  printf("Total %f\n",t_total);

  return 0;
}