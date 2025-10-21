/*
 HPC Class: OpenMP Directives, parallel, sections and parallel for
 Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>

 Description:
 This program demonstrates the basic usage of OpenMP with examples of 
 different OpenMP directives including `parallel sections`, `parallel`, 
 and `parallel for`. 

 Allocation:
 srun -N 1 --exclusive  --time=00:05:00 --reservation=hpc-monday --pty bash -i

 Compilation Instructions:
 To compile the program with OpenMP support, use the following command:
 g++ -fopenmp -o main.exe main.cpp

 Run Instructions:
 Set the environment variable OMP_NUM_THREADS to control the number of threads:
 $ export OMP_NUM_THREADS=1; ./main.exe
 
 TODO:
 1) Execute the program by setting OMP_NUM_THREADS to 1, 2, 3, and 4:
    $ export OMP_NUM_THREADS=4; ./main.exe
 2) Comment on what you expect to happen with different thread counts and 
    what you actually observe.
*/

#include <omp.h>
#include <iostream>

int main() {

    #pragma omp parallel sections
    {
        #pragma omp section
        { 
            printf("Section: thread %d/%d in section 1\n", omp_get_thread_num(), omp_get_num_threads());
        }

        #pragma omp section
        { 
            printf("Section: thread %d/%d in section 2\n", omp_get_thread_num(), omp_get_num_threads());
        }
    }


    #pragma omp parallel 
    {
        printf("Parllel: thread %d/%d in parallel region (not a section)\n", omp_get_thread_num(), omp_get_num_threads());

    }

    #pragma omp parallel for default(shared)
    for (int i = 0; i < 4; i++){
        printf("For: thread %d/%d in for loop indx %d\n", omp_get_thread_num(), omp_get_num_threads(),i);
    }

  return 0;
}

