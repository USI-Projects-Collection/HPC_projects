/*
 HPC Class: OpenMP private, firstprivate, and lastprivate
 Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

 Description:
 This program demonstrates the use of OpenMP's `private`, `firstprivate`, and 
 `lastprivate` clauses in parallel loops. These clauses control the scope and 
 initialization of variables across different threads.

 Allocation:
 srun -N 1 --exclusive  --time=00:05:00 --reservation=hpc-monday --pty bash -i

 Compilation Instructions:
 To compile the program with OpenMP support, use the following command:
 g++ -fopenmp -o main.exe main.cpp

 Run Instructions:
 Set the environment variable OMP_NUM_THREADS to control the number of threads:
 $ export OMP_NUM_THREADS=1; ./main.exe

 TODO:
 1) Run the program and observe how `private(x)` works.
 2) Modify the program to use `firstprivate(x)` (in place of `private(x)`) and observe the changes.
 3) Experiment with adding `lastprivate(x)` and see how the variable behaves.
*/

#include <omp.h>
#include <iostream>

int main() {
    int x = 10;

    #pragma omp parallel for private(x)
    for (int i = 0; i < 5; ++i) {
        x = x*2;  // Calculate squares
        
        // Output x from each thread
        #pragma omp critical 
        {
            std::cout << "Thread " << omp_get_thread_num() 
                      << ", i = " << i
                      << ": x = " << x << std::endl;
        }
    }
    // Output x from master thread (unchanged)
    std::cout << "Master thread: x = " << x << std::endl;
    
    return 0;
}
